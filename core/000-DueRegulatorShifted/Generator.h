#ifndef GENERATOR_H
#define GENERATOR_H

#include "Arduino.h"

class Generator {
  
  private:
    float Tj  = 8.81;
    float Td0 = 5.02;
    float Te  = 0.04;
    float Tu  = 0.05;
    
    float k0u = 100.f;
    
    float Xd;
    float Xds;
    float Xq;
    
    int pinU;
    int pinEqs;
  
  public:
    Generator(int pinU, int pinEqs);
    void init();
    
    void setARV(float Tj = 8.81, float Td0 = 5.02, float Te = 0.04, float Tu = 0.05, float k0u=100.f);
    void setImpedance(float Xd=2310.f, float Xds=770.f, float Xq=1500.f);
    void setParams(float delta0 = 0.6, float omega0 = 2*PI*50.f, float Eqs0 = 1.5f, 
                   float x30=0.f, float Ug0=1.9f);
    
    void iterate(float dt);
    void output();
    
    float P(float delta, float Eqs, float nu, float U);
    float Eq(float delta, float Eqs, float nu, float U);
    
    float delta0; 
    float delta;
    float omega0, omega, omega_nom;
    float Eqs0,   Eqs;
    float x30,    x3;
    float Ug0,    U;
    float Eqe0,   Eqe;  
    
    float nu = 0.f;
    
    float Pt;   
  };
  
  #endif
