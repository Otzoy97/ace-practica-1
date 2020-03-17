#include "Setup.h"
#include "RouteHandler.h"
#include "ColorSensor.h"
#include "UltraSonicSensor.h"
#include "EEPROMHandler.h"
///SIRVE PARA MANEJAR TEMPORIZADORES DIFERENTES PARA
///LEER EL COLOR Y DETERMINA LAS DISTANCIAS DE LOS OBJETOS
volatile unsigned long toReadColor = 0;
volatile unsigned long toReadDistance = 0;

//ALOJA EL VALOR DE LA ULTIMA DISTANCIA CALCULADA
volatile float auxD1 = 0;
volatile float auxD2 = 0;
volatile float auxD3 = 0;

//PERMITE ACCEDER A LAS FUNCIONES EEPROMHandler
EEPROMHandler eh_;

//LO UTILIZA EL SCANNER
struct Route r_;
struct Vector v_;
short s_ = 0;
//----{DIR, DUR}
char v_Dir = '\0';
char v_Dur = '\0';
short ct_N = 0;
short ct_V = 0;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  PinSetup();
}

void loop()
{
  // DETECTA Y ALMACENA EL COLOR EN EL QUE ESTÁ ACTUALMENTE
  // LA BANDERA ALLOWEDREADCOLOR DECIDE SI SE TOMA EN CUENTA 
  // LAS LECTURAS DEL SENSOR DE COLOR
  if (allowedReadColor && millis() > toReadColor + 500)
  {
    toReadColor = millis();
    colorRead = getColor();

    Serial.println(colorRead);
    if (colorRead == 'B')
    {
      MotorIzqMov.run(RELEASE);
      MotorDerMov.run(RELEASE);
    }
  }
  ///DETECTA OBSTÁCULOS Y DECIDE QUÉ HACER
  switch (colorRead)
  {
  case 'R': //SI ROJO -> AUTOMÁTICO
  {
    if (millis() > toReadDistance + 100)
    {
      toReadDistance = millis();
      auxD1 = GetDistanceS1();
      auxD2 = GetDistanceS2();
      auxD3 = GetDistanceS3();
      //MANTIENE PRENDID LA BARREDORA
      MotorIzqCln.run(FORWARD);
      MotorDerCln.run(BACKWARD);
      if (auxD1 <= 20 && auxD2 <= 20 && auxD3 <= 20) // Tapado para todos lados
      {
        //Cambiar la polaridad de los motores retroceder
        MotorIzqMov.run(BACKWARD);
        MotorDerMov.run(BACKWARD);
        //Serial.println("RETROCEDER");
      }
      else if (auxD2 <= 20 && auxD3 <= 20) //Tapado Frente y derecha
      {
        //Detener motor izquierdo y activar derecho
        MotorIzqMov.run(RELEASE);
        MotorDerMov.run(FORWARD);
        Serial.println("GIRAR A LA IZQ");
      }
      else if (auxD1 <= 20 && auxD2 <= 20) //Tapado Frente e Izquierda
      {
        //Detener motor derecho y activar izquierdo
        MotorIzqMov.run(FORWARD);
        MotorDerMov.run(RELEASE);
        ///Serial.println("GIRAR A LA DER");
      }
      else if (auxD2 <= 20) //Tapado enfrente
      {
        //Se mueve para la derecha
        MotorIzqMov.run(FORWARD);
        MotorDerMov.run(RELEASE);
        //Serial.println("GIRAR A LA DER - TAPADO ENFRENTE");
      }
      else
      {
        MotorIzqMov.run(FORWARD);
        MotorDerMov.run(FORWARD);
        //Serial.println("AVANZAR");
      }
    }
    break;
  }
  case 'B': //SI AZUL -> MANUAL
  {
    //DEBE ESPERAR INSTRUCCIONES DEL BLUETOOTH
    //DEBE LEER UN CHAR A LA VEZ Y PASARLOS A TRAVÉS DEL ESCANER
    if (Serial.available())
    {
      char c_ = Serial.read();
      switch (s_)
      {
      case 0:
      {
        if (c_ == '#')
        {
          //GUARDAR RUTA -> 1
          //REINICIA TODO
          s_ = 1;
          ct_N = 0;
          ct_V = 0;
          v_Dir = '\0';
          v_Dur = '\0';
          r_ = Route();
        }
        else if (c_ == '$')
        {
          //EJECUTAR COMANDO -> 4
          //REINICIA TODO
          s_ = 4;
          v_ = Vector();
        }
        break;
      }
      case 1:
      {
        //0-9 A-Z a-z ñ Ñ
        if ((c_ >= 48 && c_ <= 57) || (c_ >= 65 && c_ <= 90) || (c_ >= 97 && c_ <= 122) || (c_ == 164) || (c_ == 165))
        {
          //HACE EL NOMBRE
          if (ct_N < 6)
          {
            r_.name_[ct_N++] = c_;
          }
        }
        else if (c_ == ':' || c_ == ',')
        {
          //-----------------
        }
        else if (c_ == '{')
        {
          // COMIENZA PAR DIR, DUR -> 2
          s_ = 2;
        }
        else if (c_ == '\n')
        {
          //TERMINA EL COMANDO -> 0
          s_ = 0;
          //EJECUTA EL COMANDO GUARDAR
          executeCommand('B');
        }
        break;
      }
      case 2:
      {
        if (c_ >= 48 && c_ <= 51)
        {
          //ALMACENA UNA NUEVA DIRECCION
          v_Dir = c_;
        }
        else if (c_ == ',')
        {
          //COMIENZA LA DURACIÓN DE LA DIRECCION -> 3
          s_ = 3;
        }
        break;
      }
      case 3:
      {
        if (c_ >= 48 && c_ <= 57)
        {
          if (v_Dur != '\0') //YA SE ALMACENÓ AL MENOS UN DIGITO
          {
            ///ENTONCES ES UNA DECENA, SE MULTIPLICA EL VALOR EXISTENTE Y SE SUMA EL NUEVO DIGITO
            v_Dur *= 10;
            v_Dur += c_;
          }
          else //NO SE HA ALMACENADO NADA
          {
            v_Dur = c_;
          }
        }
        else if (c_ == '}')
        {
          //TERMINA PAR {DUR,DIR} >
          //SE ALMACENA EL VECTOR CREADO
          if (ct_V <= 8)
          {
            struct Vector nuevo;
            nuevo.dir = v_Dir;
            nuevo.dur = v_Dur;
            r_.dirs[ct_V++] = nuevo;
          }
          //SE LIMPIA
          s_ = 1;
          v_Dir = '\0';
          v_Dur = '\0';
        }
        break;
      }
      case 4:
      {
        if (c_ >= 48 && c_ <= 57 || 'A')
        {
          v_.dir = c_;
        }
        else if (c_ = '\n')
        {
          //TERMINÓ DE LEER UNA DIRECCION
          s_ = 0;
          //EJECUTA EL COMANDO ASIGNADO AL CARACTER
          executeCommand(v_.dir);
        }
        break;
      }
      }
    }
    break;
  }
  case 'N': //SI NEGRO -> RETROCEDER
  {
    //Cambiar la polaridad de los motores retroceder
    MotorIzqMov.run(BACKWARD);
    MotorDerMov.run(BACKWARD);
    break;
  }
  }
}

///HACE MOVER HACIA LA DIRECCIÓN QUE SE RECIBIÓ
void executeCommand(char cmd)
{
  switch (cmd)
  {
  case 0:
  {
    //ADELANTE
    MotorIzqMov.run(FORWARD);
    MotorDerMov.run(FORWARD);
    break;
  }
  case 1:
  {
    //ATRAS
    MotorIzqMov.run(BACKWARD);
    MotorDerMov.run(BACKWARD);
    break;
  }
  case 2:
  {
    //Detener motor derecho y activar izquierdo
    MotorIzqMov.run(FORWARD);
    MotorDerMov.run(RELEASE);
    break;
  }
  case 3:
  {
    //IZQUIERDA
    MotorIzqMov.run(RELEASE);
    MotorDerMov.run(FORWARD);
    break;
  }
  case 4:
  {
    //BARRER
    MotorIzqCln.run(FORWARD);
    MotorDerCln.run(BACKWARD);
    break;
  }
  case 5:
  {
    //LIMPIAR RUTAS
    eh_.cleanEEPROM();
    break;
  }
  case 6:
  {
    //DEVOLVER/PEDIR RUTAS
    char *toSerial = eh_.readRoutes();
    Serial.write(toSerial);
    break;
  }
  case 7:
  {
    //EJECUTRA RUTA 1
    executeRoute(eh_.readRoute(0));
    break;
  }
  case 8:
  {
    //EJECUTAR RUTA 2
    executeRoute(eh_.readRoute(1));
    break;
  }
  case 9:
  {
    //EJECUTAR RUTA 3
    executeRoute(eh_.readRoute(2));
    break;
  }
  case 'A':
  {
    //DETENER
    MotorIzqMov.run(RELEASE);
    MotorDerMov.run(RELEASE);
    break;
  }
  case 'B':
  {
    eh_.saveRoute(r_);
    break;
  }
  }
}

///EJECUTA UNA RUTA
void executeRoute(struct Route rou_)
{
  for (int i = 0 ; i < rou_.dirsCount ;i ++)
  {
    unsigned long delayRoute;
    executeCommand(rou_.dirs[i].dir);
    delayRoute = rou_.dirs[i].dur * 1000;
    delay(delayRoute);
  }
}
