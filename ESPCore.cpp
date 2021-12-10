#include <ESPCore.h>
AsyncWebServer server(80);
AsyncEventSource events("/events");
String IPADDRESS = "Waiting for ip ...";
String wifi_ssid = "waiting for ssid ...";
String project_title = "ESPCore";
long rssi = 0;
CoreLinkedList<ESPCoreVariable*> variables = CoreLinkedList<ESPCoreVariable*>();
CoreLinkedList<ESPCoreVariable*> properties = CoreLinkedList<ESPCoreVariable*>();
CoreLinkedList<ESPCoreSwitch*> switches = CoreLinkedList<ESPCoreSwitch*>();
DynamicJsonBuffer jsonBuffer;
bool shouldReboot = false;

String getJSON(CoreLinkedList<ESPCoreVariable*> items) {
    
    JsonObject& root = jsonBuffer.parseObject("{}");
    ESPCoreVariable* tmp;
    for (int i = 0; i < items.size(); i++) {
        tmp = items.get(i);
        JsonObject& part = jsonBuffer.parseObject("{}");
        part["name"] = tmp->title;
        part["unit"] = tmp->unit;
        part["value"] = tmp->getValue();
        root[tmp->key] = part;
        tmp = NULL;
    }

    String jsonreturn;
    root.printTo(jsonreturn);
    Serial.print("---");
    Serial.println(jsonreturn);
    return jsonreturn;
}


String processor(const String& var){
    if (var == "IPADDRESS") {
        return IPADDRESS;
    }
    else if (var == "PROJECT_TITLE") {
        return project_title;
    }
    else if (var == "SIGNAL") {
        return String(rssi);
    }
    else if (var == "SSID") {
        return String(wifi_ssid);
    }
    else if (var == "JSON_PROPS") {
        //return getJSON(properties);
        JsonObject& root = jsonBuffer.parseObject("{}");
        ESPCoreVariable* tmp;
        for (int i = 0; i < properties.size(); i++) {
            tmp = properties.get(i);
            JsonObject& part = jsonBuffer.parseObject("{}");
            part["name"] = tmp->title;
            part["unit"] = tmp->unit;
            part["value"] = tmp->getValue();
            root[tmp->key] = part;
            tmp = NULL;
        }

        String jsonreturn;
        root.printTo(jsonreturn);
        Serial.print("---");
        Serial.println(jsonreturn);
        return jsonreturn;
    }
    else if (var == "JSON_VARS") {
        //return getJSON(variables);
        JsonObject& root = jsonBuffer.parseObject("{}");
        ESPCoreVariable* tmp;
        for (int i = 0; i < variables.size(); i++) {
            tmp = variables.get(i);
            JsonObject& part = jsonBuffer.parseObject("{}");
            part["name"] = tmp->title;
            part["unit"] = tmp->unit;
            part["value"] = tmp->getValue();
            root[tmp->key] = part;
            tmp = NULL;
        }

        String jsonreturn;
        root.printTo(jsonreturn);
        Serial.print("---");
        Serial.println(jsonreturn);
        return jsonreturn;
    }
    else if (var == "JSON_SWITCHES") {
        //return getJSON(variables);
        JsonObject& root = jsonBuffer.parseObject("{}");
        ESPCoreSwitch* tmp;
        for (int i = 0; i < switches.size(); i++) {
            tmp = switches.get(i);
            JsonObject& part = jsonBuffer.parseObject("{}");
            part["name"] = tmp->title;
            part["color"] = tmp->color;
            part["value"] = tmp->value;
            root[tmp->key] = part;
            tmp = NULL;
        }

        String jsonreturn;
        root.printTo(jsonreturn);
        Serial.print("---");
        Serial.println(jsonreturn);
        return jsonreturn;
    }
  return String();
}

ESPCore::ESPCore(String title, int sync_delay)
{
    project_title = title;
    s_delay = sync_delay;
    Serial.println("initialised");
}

ESPCore::ESPCore(String title)
{
    project_title = title;
    s_delay = 1000;
    Serial.println("initialised");
}

void ESPCore::startServer() {

    server.on("/test", HTTP_GET, [](AsyncWebServerRequest* request) {
        Serial.println("TEST REQUEST");
        request->send(200, "text/html", "<html><body>test<body/></html>");
    });

    server.on("/switch-action", HTTP_POST, [](AsyncWebServerRequest* request) {
        AsyncWebParameter* p = request->getParam("state");
        AsyncWebParameter* q = request->getParam("key");
        ESPCoreSwitch* tmp;
        for (int i = 0; i < switches.size(); i++) {
            tmp = switches.get(i);
            if (tmp->key == String(q->value())) {
                if (String(p->value()) == "true") {
                    tmp->callAction(true);
                }else {
                    tmp->callAction(false);
                }
            }
            String key_state = tmp->key + "_state";
            events.send("false", key_state.c_str(), millis());
        }


        
        request->send(200, "application/json", "{\"state\":true}");
    });

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", _DASHBOARD_HTML, processor);
    });

    server.on("/info", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send_P(200, "text/html", _INFO_HTML, processor);
    });

    server.on("/logs", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send_P(200, "text/html", _LOGS_HTML, processor);
        });

    server.on("/update", HTTP_GET, [](AsyncWebServerRequest* request) {
        //ESPCoreContent content(1);
        request->send_P(200, "text/html", _UPDATE_HTML, processor);
    });

    server.on("/ping", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(200, "text/plain", "true");
        });

    server.on("/update", HTTP_POST, [](AsyncWebServerRequest* request) {
        //shouldReboot = !Update.hasError();
        AsyncWebServerResponse* response = request->beginResponse_P(200, "text/html", (Update.hasError()) ? _UPDATING_HTML : _UPDATING_HTML, processor);
        response->addHeader("Connection", "close");
        request->send(response);
        }, [](AsyncWebServerRequest* request, String filename, size_t index, uint8_t* data, size_t len, bool final) {
            if (!index) {
                Serial.printf("Update Start: %s\n", filename.c_str());
                Update.runAsync(true);
                if (!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000)) {
                    Update.printError(Serial);
                }
            }
            if (!Update.hasError()) {
                if (Update.write(data, len) != len) {
                    Update.printError(Serial);
                }
            }
            if (final) {
                if (Update.end(true)) {
                    shouldReboot = true;
                    Serial.printf("Update Success: %uB\n", index + len);
                }
                else {
                    Update.printError(Serial);
                }
            }
        });
    
    events.onConnect([](AsyncEventSourceClient* client) {
        if (client->lastId()) {
            Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
        }
        client->send("hello!", NULL, millis(), 10000);
        });
    server.addHandler(&events);
    
    server.begin();
    Serial.println("HTTP server started");
}

void ESPCore::registerSwitch(String title, String active_colour, String key, void(*function)(bool))
{
    ESPCoreSwitch* v = new ESPCoreSwitch();
    v->title = title;
    v->color = active_colour;
    v->key = key;
    v->function = function;
    switches.add(v);
}

void ESPCore::updateSwitch(String key, bool state)
{
    ESPCoreSwitch* tmp;
    for (int i = 0; i < switches.size(); i++) {
        tmp = switches.get(i);
        if (tmp->key == key) {
            tmp->setValue(true);
        }
        String state = "false";
        if (state) {
            state = "true";
        }
        events.send(state.c_str(), tmp->key.c_str(), millis());
    }
}
/*
void ESPCore::registerButton(String title, String active_colour, String key, void(*function)(bool))
{
    ESPCoreVariable* v = new ESPCoreVariable();
    v->title = title;
    v->unit = unit;
    v->key = key;
    v->function = function;
    variables.add(v);
    index++;
}

void ESPCore::registerAction(String title, String active_colour, String key, void(*function)(bool))
{
    ESPCoreVariable* v = new ESPCoreVariable();
    v->title = title;
    v->unit = unit;
    v->key = key;
    v->function = function;
    variables.add(v);
    index++;
}
*/

void ESPCore::report(String title, String unit, String key, String (*function)(void))
{
    ESPCoreVariable* v = new ESPCoreVariable();
    v->title = title;
    v->unit = unit;
    v->key = key;
    v->function = function;
    variables.add(v);
    index++;
}

void ESPCore::update(String key, String value)
{
    events.send(value.c_str(), key.c_str(), millis());
}

void ESPCore::log(String log_message)
{
    Serial.print("debug-- ");
    Serial.println(log_message);
    String json = "{\"message\":\"" + log_message + "\", \"type\":\"debug\"}";
    events.send(json.c_str(), "logger", millis());
}

void ESPCore::warn(String log_message)
{
    Serial.print("warning-- ");
    Serial.println(log_message);
    String json = "{\"message\":\"" + log_message + "\", \"type\":\"warn\"}";
    events.send(json.c_str(), "logger", millis());
}

void ESPCore::error(String log_message)
{
    Serial.print("error-- ");
    Serial.println(log_message);
    String json = "{\"message\":\"" + log_message + "\", \"type\":\"error\"}";
    events.send(json.c_str(), "logger", millis());
}

void ESPCore::reportProperty(String title, String unit, String key, String(*function)(void))
{
    ESPCoreVariable* v = new ESPCoreVariable();
    v->title = title;
    v->unit = unit;
    v->key = key;
    v->function = function;
    properties.add(v);
}

void ESPCore::loop()
{
    if ((millis() - lastTime) > s_delay) {
        lastTime = millis();
        ESPCoreVariable* tmp;
        for (int i = 0; i < variables.size(); i++) {
            tmp = variables.get(i);
            //Serial.print(tmp->title);
            //Serial.print("---");
           //Serial.println(tmp->getValue());
            events.send(tmp->getValue().c_str(), tmp->key.c_str(), millis());
        }
    }

    if ((millis() - lastSignalCheckTime) > SignalCheckTimerDelay) {
        lastSignalCheckTime = millis();
        IPADDRESS = WiFi.localIP().toString().c_str();
        rssi = WiFi.RSSI();
        wifi_ssid = WiFi.SSID();

        events.send("ping",NULL,millis());
        events.send(wifi_ssid.c_str(),"ssid",millis());
        events.send(String(rssi).c_str(),"rssi",millis());
        events.send(IPADDRESS.c_str(),"ipaddress",millis());
        ESPCoreVariable* tmp;
        for (int i = 0; i < properties.size(); i++) {
            tmp = properties.get(i);
            events.send(tmp->getValue().c_str(), tmp->key.c_str(), millis());
        }
    }
    

    if (shouldReboot) {
        Serial.println("Rebooting...");
        delay(100);
        ESP.restart();
    }
}