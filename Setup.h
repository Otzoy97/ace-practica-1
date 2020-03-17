#ifndef SETUP_H
#define SETUP_H

#include <AFMotor.h>

#define TRIGGERS1 21
#define ECHOS1 24
#define TRIGGERS2 25
#define ECHOS2 26
#define TRIGGERS3 27
#define ECHOS3 28
#define S0 30
#define S1 31
#define S2 33
#define S3 34
#define SOUT 32
#define BTNMNL 36

//MOTORES DE MOVIMEINTO
AF_DCMotor MotorIzqMov(1);
AF_DCMotor MotorDerMov(2);
//MOTORES LIMPIEZA
AF_DCMotor MotorIzqCln(4);
AF_DCMotor MotorDerCln(3);
//
volatile unsigned long btnPressed = 0;
//DETERMINA EL ESTADO EN EL QUE DEBERÁ ESTAR EL CARRO
volatile char colorRead;
//DETERMINA SI EL ESTADO COLORREAD SE TOMARÁ EN CUENTA
volatile boolean allowedReadColor = true;;

///AL PRESIONAR POR MENOS DE MEDIO SEGUNDO EL BOTON
///SE ENTRARÁ AL MODO MANUAL
///AL PRESIONAR POR MÁS DE MEDIO SEGUNDO EL BOTÓN
///SE ENTRARÁ AL MODO AUTOMÁTICO
void btnPressStartManual()
{
    if (digitalRead(BTNMNL) == HIGH)
    {
        btnPressed = millis();
    }
    else 
    {
        if (millis() - btnPressed < 500)
        {
            allowedReadColor = false;
            colorRead = 'B';
            btnPressed = 0;
        } 
        else if (millis() - btnPressed > 500)
        {
            allowedReadColor = true;
        }
        
    }
}

void PinSetup()
{
    //US1
    pinMode(TRIGGERS1, OUTPUT);
    pinMode(ECHOS1, INPUT);
    //US2
    pinMode(TRIGGERS2, OUTPUT);
    pinMode(ECHOS2, INPUT);
    //US3
    pinMode(TRIGGERS3, OUTPUT);
    pinMode(ECHOS3, INPUT);
    //CS
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(SOUT, INPUT);
    // Setting frequency-scaling to 20%
    digitalWrite(S0,HIGH);
    digitalWrite(S1,LOW);
    //ESTABLECE LA VELOCIDAD DE LOS MOTORES
    MotorIzqMov.setSpeed(200);
    MotorDerMov.setSpeed(200);
    MotorIzqCln.setSpeed(50);
    MotorDerCln.setSpeed(50);
    //ESTABLECE LA INTERRUPCIÓN AL PIN 21
    attachInterrupt(digitalPinToInterrupt(BTNMNL), btnPressStartManual, CHANGE);
}

#endif
