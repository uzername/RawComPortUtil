/* 
 * File:   ModBusHandler.h
 * Author: Ivan
 *
 * Created on 3 августа 2017 г., 0:22
 */

#ifndef MODBUSHANDLER_H
#define MODBUSHANDLER_H
#include "tty.h"

class ModBusHandler {
public:
    ModBusHandler();
    ModBusHandler(const ModBusHandler& orig);
    ModBusHandler(tty* in_serialConnection);
    virtual ~ModBusHandler();
    tty* serialConnectionInstance;
    
private:

};

#endif /* MODBUSHANDLER_H */

