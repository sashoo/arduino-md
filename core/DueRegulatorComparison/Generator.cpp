#include "Generator.h"
#include "Arduino.h"
#include "VoltConvert.h"
#include "spi_helper.h"
#include "waveform.h"
#include "common.h"

#include <math.h>

#define SQRT2 sqrt(2)
#define INV_AMP 0.4285496f // 1/3.3*sqrt(2)/1000.f
//#define FV_AMP V_AMP/1000.f
//#define FV_AMP 2.047f


Generator::Generator(float voltage_level, int output_type, int pin_output, int pin_feedback, int pin_ld) {
  this->voltage_level = voltage_level;
  this->output_type   = output_type;
  this->pin_output    = pin_output;
  this->pin_feedback  = pin_feedback;
  this->pin_ld        = pin_ld;
}

void Generator::init() {
  this->setARV();
  this->setImpedance();
  this->setParams();
}

void Generator::setARV(float Tj, float Td0, float Te, float Tu, float Tw, float Tt, float k0u, float k0w) {
  this->Tj  = Tj;
  this->Td0 = Td0;
  this->Te  = Te;
  this->Tu  = Tu;
  this->Tw  = Tw;
  this->Tt  = Tt;
  this->k0u = k0u;
  this->k0w = k0w;
}

void Generator::setImpedance(float Xd, float Xds, float Xq) {
  this->Xd  = Xd;
  this->Xds = Xds;
  this->Xq  = Xq;
}

void Generator::setParams(float delta0, float omega0, float Eqs0, float x30, float Ug0) {
  this->delta0 = this->delta = delta0;
  this->omega0 = this->omega = this->omega_nom = omega0;
  this->Eqs0   = this->Eqs   = Eqs0;
  this->x30    = this->x3    = x30;
  this->Ug0    = this->U     = Ug0;
  this->Eqe0   = this->Eqe   = this->Eq(delta0, Eqs0, 0, Ug0);
  //this->Pt     = this->P(delta0, Eqs0, 0, Ug0);
}

float Generator::P(float delta, float Eqs, float nu, float U) {
  return Eqs*U/this->Xds*sin(delta-nu) - U*U/2*(this->Xq-this->Xds)/this->Xq/this->Xds*sin(2*(delta-nu));
}

float Generator::Pe(float V) {
  float shunt_drop = volts(analogRead(A0), this->voltage_level, 12);
  float I = shunt_drop / 120.f;
  return I * V;  
}

float Generator::Eq(float delta, float Eqs, float nu, float U) {
  return Eqs*this->Xd/this->Xds - U*(this->Xd-this->Xds)/this->Xds*cos(delta - nu);
}

    
void Generator::iterate(float dt = 0.01) {
  // FIXME: hardcoded DAC resolution
  //float U     = this->U = volts(analogRead(this->pin_feedback), this->voltage_level, 12);
  float U     = this->U = volts(analogRead(this->pin_feedback), this->voltage_level, 12);
  float Eqs   = this->Eqs;
  float nu    = this->nu;
  float delta = this->delta;
  //this->power = this->P(delta, Eqs, nu, U);
  this->power = this->Pe(U);
  
  
  this->delta += dt*(this->omega - this->omega0);
  this->omega += dt*this->omega_nom/this->Tj*(this->Pt - this->power);  
  this->Eqs   += dt/this->Td0*(this->Eqe - this->Eq(delta, Eqs, nu, U)); 
  this->Eqe   += dt/this->Te*(this->x3-this->Eqe);
  this->x3    += dt/this->Tu*( this->k0u*(this->Ug0 - U) - this->x3);

  // speed regulator
  this->q  += dt/this->Tw * ( this->k0w*(this->omega0 - this->omega) - this->q );
  this->Pt += dt/this->Tt * ( this->q - this->Pt );
  
  if (this->Pt < 0)
    this->Pt = 0.f;

  //analogWrite(this->pinEqs, level(this->Eqs));
  //output(this->Eqs);
  this->Eqs = constrain(this->Eqs, 0, 3.3);
  
  //this->output(this->Eqs);
  
  //float m_power = map(this->power, 0, 3.3f)
  
  // auxillary output for chart comparison
  //analogWrite(DAC0, level(this->Pt*1000.f, 3.3f, 12)); 
  //analogWrite(DAC1, level((this->omega/this->omega_nom)*1.0f, 3.3f, 12));
  
  
  //analogWrite(DAC1, level(this->omega/this->omega_nom*2.f, 3.3f, 12));
}

void Generator::log_data() {
  SerialUSB.print('P: ');
  SerialUSB.println(this->power);
  SerialUSB.print('omega: ');
  SerialUSB.println(this->omega);
  SerialUSB.print('U: ');
  SerialUSB.println(this->U);
  SerialUSB.print('Eqs: ');
  SerialUSB.println(this->Eqs);
  SerialUSB.println('***');
}

// outputs voltage; takes voltage level (3.3V or 5.0 into consideration)
void Generator::output(float value) {
  // FIXME: hardcoded resolution
  

  if (this->output_type == DAC_INTERN) {
    int lvl = level(value, this->voltage_level, 12);
    //analogWrite(this->pin_output, lvl);
    analogWrite(this->pin_output, lvl);
  }
  else if (this->output_type == DAC_EXTERN) {
    int lvl = level_8512(value);
    digitalWrite(this->pin_ld, HIGH);
    SPI_send12(this->pin_output, lvl);
    digitalWrite(this->pin_ld, LOW);
  }
}

void Generator::wave_output(float value) {
  // FIXME: hardcoded resolution
  int local_step = wave_step + this->wave_shift;
  if (local_step >= PHI_STEPS) local_step = 0;  
  
  //value = this->Eqs * INV_AMP * sine_wave[local_step];
  value = 1.5f * INV_AMP * sine_wave[local_step];
  

  if (this->output_type == DAC_INTERN) {
    //int lvl = level(value, this->voltage_level, 12);
    //analogWrite(this->pin_output, lvl);
    analogWrite(this->pin_output, int(value));
  }
  else if (this->output_type == DAC_EXTERN) {
    int lvl = level_8512(value);
    digitalWrite(this->pin_ld, HIGH);
    SPI_send12(this->pin_output, lvl);
    digitalWrite(this->pin_ld, LOW);
  }
}
