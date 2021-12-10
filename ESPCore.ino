/*
  ESPCore.h - Library for maintaining and viewing data comming from an esp device.
  Created by Dustin Auby, October 14, 2021.
  Released into the public domain.
*/

#ifndef ESPCore_h
#define ESPCore_h

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <RemoteDebug.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <FS.h>

float *p;

class ESPCore{
  public:
    ESPCore()
    {
      Serial.println("initialised");
    }
  
    void report(float &variable)
    {
      *p = variable;
      Serial.println("added pointer");
    }

    void loop()
    {
        Serial.println("looped");
        Serial.println(*p);
    }
};
#endif
