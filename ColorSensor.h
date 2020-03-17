#ifndef COLORSENSOR_H
#define COLORSENSOR_H
#include "Setup.h"
volatile unsigned long  auxDelayColor = 0;

char getColor()
{
    int freq[3];
    // Setting red filtered photodiodes to be read
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    freq[0] = pulseIn(SOUT, LOW);
    auxDelayColor = millis();
    while(millis() < auxDelayColor + 50){}
    // Setting Green filtered photodiodes to be read
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    freq[1] = pulseIn(SOUT, LOW);
    auxDelayColor = millis();
    while(millis() < auxDelayColor + 50){}
    // Setting Blue filtered photodiodes to be read
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    freq[2] = pulseIn(SOUT, LOW);
    Serial.print(freq[0]);
    Serial.print(",");
    Serial.print(freq[1]);
    Serial.print(",");
    Serial.print(freq[2]);
    
    if ( ( freq[0] >= 94 && freq[0] <= 98 ) && ( freq[1] >= 118  && freq[1] <= 122 ) && ( freq[2] >= 102 && freq[2] <= 104 ) )
    {
      return 'R';
    }
    else if ( ( freq[0] >= 119 && freq[0] <= 121  ) && ( freq[1] >= 114 && freq[1] <= 117 ) && ( freq[2] >= 93 && freq[2] <= 95 ) )
    {
      return 'B';
    }
    else if ( ( freq[0] >= 119 && freq[0] <= 133 ) && ( freq[1] >= 121 && freq[1] <= 136  ) && ( freq[2] >= 105 && freq[2] <= 117) )
    {
      return 'N';
    }
    return 'V';
}

#endif
