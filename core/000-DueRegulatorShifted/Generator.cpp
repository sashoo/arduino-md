#include "Generator.h"
#include "Arduino.h"
#include "VoltConvert.h"

#include <math.h>


Generator::Generator(int pinU, int pinEqs) {
  this->pinU = pinU;  
  this->pinEqs = pinEqs; 
}

void Generator::init() {
  this->setARV();
  this->setImpedance();
  this->setParams();
}

void Generator::setARV(float Tj, float Td0, float Te, float Tu, float k0u) {
  this->Tj  = Tj;
  this->Td0 = Td0;
  this->Te  = Te;
  this->Tu  = Tu;
  
  this->k0u = k0u;
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
  this->Pt     = this->P(delta0, Eqs0, 0, Ug0);
}

float Generator::P(float delta, float Eqs, float nu, float U) {
  return Eqs*U/this->Xds*sin(delta-nu) - U*U/2*(this->Xq-this->Xds)/this->Xq/this->Xds*sin(2*(delta-nu));
}

float Generator::Eq(float delta, float Eqs, float nu, float U) {
  return Eqs*this->Xd/this->Xds - U*(this->Xd-this->Xds)/this->Xds*cos(delta - nu);
}

    
void Generator::iterate(float dt = 0.01) {
  float U     = this->U = volts(analogRead(this->pinU));
  float Eqs   = this->Eqs;
  float nu    = this->nu;
  float delta = this->delta;
  
  
  this->delta += dt*(this->omega - this->omega0);
  this->omega += dt*this->omega_nom/this->Tj*(this->Pt - this->P(delta, Eqs, nu, U));  
  this->Eqs   += dt/this->Td0*(this->Eqe - this->Eq(delta, Eqs, nu, U)); 
  this->Eqe   += dt/this->Te*(this->x3-this->Eqe);
  this->x3    += dt/this->Tu*( this->k0u*(this->Ug0 - U) - this->x3);

  analogWrite(this->pinEqs, level(this->Eqs));
}

void Generator::output() {
  //analogWrite(this->pinEqs, level(this->Eqs));
}
