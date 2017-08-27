/* 
 * File:   ModBusHandler.h
 * Author: Ivan
 *
 * Created on 3 августа 2017 г., 0:22
 */

#ifndef MODBUSHANDLER_H
#define MODBUSHANDLER_H
#include "tty.h"

#include <windows.h>

class ModBusHandler {
public:
    ModBusHandler();
    ModBusHandler(const ModBusHandler& orig);
    ModBusHandler(tty* in_serialConnection);
    virtual ~ModBusHandler();
    tty* serialConnectionInstance;
    void startRS232Queries();
    
private:
    void closeThreadHandles();
    DWORD ThreadProc(CONST LPVOID lpParam);
    uint8_t proceedMonitoring;
    HANDLE hMutex;
    HANDLE hQueryThread;
};

#endif /* MODBUSHANDLER_H */

