#include <ESPCoreVariable.h>


ESPCoreVariable::ESPCoreVariable(){

}

String ESPCoreVariable::getValue() {
    return String((*function)());
}

void ESPCoreVariable::setValue(String data) {
    value = data;
}