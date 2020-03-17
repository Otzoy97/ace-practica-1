#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H
#include "Setup.h"

unsigned volatile long DelayDistanceAux = 0;

float GetDistanceS1()
{

  float Duration;
  digitalWrite(TRIGGERS1, LOW);
  DelayDistanceAux = micros();
  while (micros() < DelayDistanceAux + 2) { }
  digitalWrite(TRIGGERS1, HIGH);
  DelayDistanceAux = micros();
  while (micros() < DelayDistanceAux + 10) { }
  digitalWrite(TRIGGERS1, LOW);
  //Obtiene la lectura de la distancia del ultrasonido
  Duration = pulseIn(ECHOS1, HIGH);
  //Calcula la distancia
  return (Duration * 0.03432 / 2);
}

float GetDistanceS2()
{
  float Duration;
  digitalWrite(TRIGGERS2, LOW);
  DelayDistanceAux = micros();
  while (micros() < DelayDistanceAux + 2)  { }
  digitalWrite(TRIGGERS2, HIGH);
  DelayDistanceAux = micros();
  while (micros() < DelayDistanceAux + 10) { }
  digitalWrite(TRIGGERS2, LOW);
  //Obtiene la lectura de la distancia del ultrasonido
  Duration = pulseIn(ECHOS2, HIGH);
  //Calcula la distancia
  return (Duration * 0.03432 / 2);
}

float GetDistanceS3()
{
  float Duration;
  digitalWrite(TRIGGERS3, LOW);
  DelayDistanceAux = micros();
  while (micros() < DelayDistanceAux + 2) {  }
  digitalWrite(TRIGGERS3, HIGH);
  DelayDistanceAux = micros();
  while (micros() < DelayDistanceAux + 10) { }
  digitalWrite(TRIGGERS3, LOW);
  //Obtiene la lectura de la distancia del ultrasonido
  Duration = pulseIn(ECHOS3, HIGH);
  //Calcula la distancia
  return (Duration * 0.03432 / 2);
}

#endif
