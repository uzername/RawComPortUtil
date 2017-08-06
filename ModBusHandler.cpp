/* 
 * File:   ModBusHandler.cpp
 * Author: Ivan
 * Used to handle modbus protocol using serial connection
 */

#include "ModBusHandler.h"

ModBusHandler::ModBusHandler() {
    
}

ModBusHandler::ModBusHandler(tty* in_serialConnection) {
    if(serialConnectionInstance != NULL) {
        serialConnectionInstance->Disconnect();
    }
    serialConnectionInstance = in_serialConnection;
}

ModBusHandler::ModBusHandler(const ModBusHandler& orig) {
    
}

ModBusHandler::~ModBusHandler() {
    if(serialConnectionInstance != NULL) {
        serialConnectionInstance->Disconnect();
        //free(serialConnectionInstance);
    }
}

