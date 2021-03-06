#ifndef GENERATOR_H
#define GENERATOR_H

#include "Arduino.h"

#define DAC_INTERN 0 // Due DAC
#define DAC_EXTERN 1 // Texas Instruments DAC8512 as of now

class Generator {
  
  private:
  
    // *** regulator parameters ***
    float Tj  = 8.81;
    float Td0 = 5.02;
    float Te  = 0.04;
    float Tu  = 0.05;    
    float Tw  = 0.05;    
    float Tt  = 0.80;    
    float k0u = 100.f;
    float k0w = 1.0; // FIXME
    
    // *** generator impedances ***
    float Xd;
    float Xds;
    float Xq;

    // *** Due pins ***
    
    // voltage level of the DAC
    // (3.3V for Due DAC, 5.0V for DAC8512)
    int voltage_level; 

    // type of DAC used (DAC_INTERN or DAC_EXTERN)
    int output_type; 
    
    // pin that measures voltage and acts as measurement feedback
    int pin_feedback; 
    

  
  public:
    Generator(float voltage_level, int output_type, int pin_output, int pin_feedback, int pin_ld=0);
    void init();
    void log_data();
    
    // set regulator parameters
    void setARV(float Tj = 8.81, float Td0 = 5.02, float Te =  0.04, float Tu  = 0.05,
                float Tw = 0.05, float Tt  = 0.8,  float k0u=100.f,  float k0w  =0.2f);
    // set generator impedances
    void setImpedance(float Xd=2310.f, float Xds=770.f, float Xq=1500.f);
    void setParams(float delta0 = 0.6, float omega0 = 2*PI*50.f, float Eqs0 = 1.0f, 
                   float x30=0.f, float Ug0=1.0f);
    
    void iterate(float dt);
    void output(float value);
    void wave_output(float value);
    
    float P(float delta, float Eqs, float nu, float U);
    float Eq(float delta, float Eqs, float nu, float U);
    float Pe(float V);
    
    float delta0; 
    float delta;
    float omega0, omega, omega_nom;
    float Eqs0,   Eqs;
    float x30,    x3;
    float Ug0,    U;
    float Eqe0,   Eqe;  
    float power;
    
    int wave_shift = 0;

    float Pt = 0.f;
    float q  = 0.f;
    
    float nu = 0.f; 
    
        // chip-select pin: 
    // * for internal (DAC_INTERN) Due DAC it's a DAC{0, 1},   
    // * for external (DAC_EXTERN) DAC it's any digital pin 
    //   used as chip-select/slave select (CS/SS) 
    int pin_output;    
    
    // pin that writes data to output register of DAC8512
    // (strobe pin)
    // exclusive to DAC_EXTERN output with DAC8512 in use
    int pin_ld;
  };
  
#endif
