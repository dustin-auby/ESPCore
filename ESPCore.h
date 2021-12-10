/*
  ESPCore.h - Library for maintaining and viewing data comming from an esp device.
  Created by Dustin Auby, October 14, 2021.
  Released into the public domain.
*/

#ifndef ESPCore_h
#define ESPCore_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <FS.h>

#include <CoreLinkedList.h>
#include <ESPCoreContentUpdate.h>
#include <ESPCoreContentInfo.h>
#include <ESPCoreContentDashboard.h>
#include <ESPCoreContentLogs.h>
#include <ESPCoreContentUpdating.h>
#include <ESPCoreVariable.h>
#include <ESPCoreSwitch.h>


class ESPCore {
private:
    
    int index = 0;
    int s_delay;
    unsigned long lastTime = 0;
    
    
    unsigned long SignalCheckTimerDelay = 5000;
    unsigned long lastSignalCheckTime = 0;

public:
    


    ESPCore(String title, int sync_delay);
    ESPCore(String title);
    
    void startServer();
    void update(String key, String value);
    void log(String log_message);
    void warn(String log_message);
    void error(String log_message);
    void report(String title, String unit, String key, String(*function)());
    void reportProperty(String title, String unit, String key, String(*function)());
    
    void registerSwitch(String title, String active_colour, String key, void(*function)(bool));
    void updateSwitch(String key, bool state);

    void loop();
};
#endif