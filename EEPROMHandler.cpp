#include "EEPROMHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


EEPROMHandler::EEPROMHandler() { }

void EEPROMHandler::cleanEEPROM()
{
    ///LIMPIA LA EEPROM
    for(int i = 0; i < EEPROM.length() ; i++)
    {
        EEPROM.write(i, 255);
    }
}

bool EEPROMHandler::saveRoute(struct Route r_)
{
    ///VALIDA QUE NO SE REPITA EL NOMBRE
    for (int i = 0; i < 3 ; i++)
    {
        if (existRoute(r_.name_))
        {
            return false;
        }
    }
    ///BUSCA UN LUGAR DONDE METER LA RUTA
    for (int i = 0; i < 3; i++)
    {
        if (EEPROM.read(i * sizeof(Route)) == 255)
        {
           EEPROM.put(i * sizeof(Route), r_); 
           return true;
        }            
    }
    return false;
}

char* EEPROMHandler::readRoutes()
{
    char routesNames[22];
    struct Route tmp;
    for (int i = 0 ; i < 22 ; i++)
    {
        routesNames[i] = '\0';
    }
    ///=============================
    for (int i = 0 ; i < 3 ; i++)
    {
        ///LEE EL PRIMER BYTE 
        if (EEPROM.read(i * sizeof(Route)) != 255)
        {
            ///SI NO ES "NULO" -> ENTONCES LEER
            
            EEPROM.get( i * sizeof(Route), tmp );    
            ///CONCATENA EL NOMBRE DE LA RUTA
            strcat(routesNames, tmp.name_);
            strcat(routesNames, ",");
        }
        
    }
    strcat(routesNames, "#");
    return routesNames;
}

struct Route EEPROMHandler::readRoute(int idx_)
{
    struct Route tmp;
    if (EEPROM.read( idx_ * sizeof(Route)) != 255 )
    {
        EEPROM.get( idx_ * sizeof(Route), tmp );
    }
    return tmp;
}

bool EEPROMHandler::existRoute(char *n_)
{
    struct Route tmp;
    for (int i = 0 ; i < 3 ; i++)
    {
        if (EEPROM.read(i * sizeof(Route)) != 255)
        {
            EEPROM.get( i * sizeof(Route), tmp );
            if (strcmp(tmp.name_, n_) == 0) 
            {
                //SI EL NOMBRE COINCIDE = TRUE
                return true;
            }
        }
    }
    return false;
}
