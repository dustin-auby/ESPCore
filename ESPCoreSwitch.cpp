#include <ESPCoreSwitch.h>


ESPCoreSwitch::ESPCoreSwitch(){

}

void ESPCoreSwitch::callAction(bool state) {
    (*function)(state);
}

void ESPCoreSwitch::setValue(bool data) {
    value = data;
}