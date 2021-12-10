/*
  ESPCore.h - Library for maintaining and viewing data comming from an esp device.
  Created by Dustin Auby, October 14, 2021.
  Released into the public domain.
*/

#ifndef ESPCoreVariable_h
#define ESPCoreVariable_h

#include <Arduino.h>

class ESPCoreVariable {
public:
    String value;
    String title;
    String unit;
    String key;

    String (*function)(void);

    ESPCoreVariable();
    String getValue();
    void setValue(String data);
};


#endif