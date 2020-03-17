#ifndef EEPROMHANDLER_H
#define EEPROMHANDLER_H
#include <EEPROM.h>
#include "RouteHandler.h"
#include "Arduino.h"

class EEPROMHandler
{
public:
    EEPROMHandler();        
    ///ELIMINA TODAS LAS RUTAS ALMACENADAS EN LA EEPROM
    void cleanEEPROM();
    ///ALMACENA UNA RUTA EN LA MEMORIA EEPROM
    bool saveRoute(struct Route);
    ///LEE TODAS LAS RUTAS Y DEVUELVE SUS NOMBRES
    char* readRoutes();
    ///LEE LA RUTA ALMACENADA DADA UN NOMBRES
    struct Route readRoute(int);
    ///VERIFICA QUE EXISTA UNA RUTA CON EL NOMBRE DADO
    bool existRoute(char*);
    ///CUENTA EL NUMERO DE RUTAS QUE HAY ALMACENADAS
    void countRoutes();
private:
    ///MANTIENE UN CONTADOR DE TODAS LAS RUTAS GUARDADAS
    volatile int short routeCounter = 0;
};

#endif
