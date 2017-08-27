/* 
 * File:   ModBusHandler.cpp
 * Author: Ivan
 * Used to handle modbus protocol using serial connection
 */

#include "ModBusHandler.h"

ModBusHandler::ModBusHandler() {
    serialConnectionInstance = NULL;
    hMutex = INVALID_HANDLE_VALUE;
    hQueryThread = INVALID_HANDLE_VALUE;
    this->proceedMonitoring = 0;
}

ModBusHandler::ModBusHandler(tty* in_serialConnection) {
    /*
    if(serialConnectionInstance != NULL) {
        serialConnectionInstance->Disconnect();
    }
    */
    serialConnectionInstance = in_serialConnection;
    hMutex = INVALID_HANDLE_VALUE;
    hQueryThread = INVALID_HANDLE_VALUE;
    this->proceedMonitoring = 0;
}

ModBusHandler::ModBusHandler(const ModBusHandler& orig) {
    
}

ModBusHandler::~ModBusHandler() {
    if(serialConnectionInstance != NULL) {
        serialConnectionInstance->Disconnect();
        //free(serialConnectionInstance);
    }
}
/**
 * A routine which is called in multithreaded routine
 * @param lpParam - pointer to mutex which is used to block queries to shared port resource
 * @return 
 */
ModBusHandler::ThreadProc(CONST LPVOID lpParam) {
  CONST HANDLE hMutex = (CONST HANDLE)lpParam;
  while (this->proceedMonitoring == 1) {
    WaitForSingleObject(hMutex, INFINITE);
    vector<unsigned char> vec;
    serialConnectionInstance->Read(vec);
    if (vec.size()>0) {
        //we have some data right here!
    }
    ReleaseMutex(hMutex);
  }
  ExitThread(0);
}

/**
 * configure monitoring of the rs232 port in multithread manner
 * http://eax.me/winapi-threads/
 * @return 
 */
ModBusHandler::startRS232Queries() {
    this->closeThreadHandles();
    hMutex = CreateMutex(NULL, FALSE, NULL);
    hQueryThread = CreateThread(NULL, 0, &ThreadProc, hMutex, 0, NULL);
    
}

ModBusHandler::closeThreadHandles() {
    if (hMutex != INVALID_HANDLE_VALUE) {
        CloseHandle(hMutex);
        hMutex = INVALID_HANDLE_VALUE;
    }
    if (hQueryThread != INVALID_HANDLE_VALUE) {
        CloseHandle(hQueryThread);
        hQueryThread = INVALID_HANDLE_VALUE;
    }
}