/*
  ESPCore.h - Library for maintaining and viewing data comming from an esp device.
  Created by Dustin Auby, October 14, 2021.
  Released into the public domain.
*/

#ifndef ESPCoreSwitch_h
#define ESPCoreSwitch_h

#include <Arduino.h>

class ESPCoreSwitch {
public:
    bool value;
    String title;
    String color;
    String key;

    void (*function)(bool);

    ESPCoreSwitch();
    void setValue(bool data);
    void callAction(bool state);
};


#endif