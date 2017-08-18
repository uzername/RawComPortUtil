#include <cstdlib>
#include <cstdio>
#include <memory>
#include "tty.h"
#include "ModBusHandler.h"
using namespace std;
int main(int argc, char** argv) {
    printf("===COM PORT WINDOWS UTIL===\n");
    //add comport enumeration using enumser module
    tty* COMportInstance = new tty();
    printf("port: ");
    char portName[100];
    int totalSymbols = scanf("%99[^\n]", portName);
    if (totalSymbols == 0) {return 0;}
    string* portnameString = new string(portName, strlen(portName) );
    ModBusHandler* processModbus;
    //printf("%s %s", "Your line: ", portnameString->c_str() );
    try {
        COMportInstance->Connect(portnameString,9600, 8, 1, NONE);
        processModbus = new ModBusHandler(COMportInstance);
        printf("Connection OK. Modbus instance OK.\n");
         static const unsigned char arr[] = {1,3,2,4};
         vector<unsigned char> vec (arr, arr + sizeof(arr) / sizeof(arr[0]) );
         COMportInstance->Write(vec);
         printf("data has been written to port\n");
         vec[0]=20; vec[1]=30; vec[2]=40; vec[3]=50;
         COMportInstance->Write(vec);
         printf("data has been written to port\n");
    } catch (TTYException e) {
        printf("\n %s",e.exception_cause->c_str());
        COMportInstance->Disconnect();
        return 1;
    }
    
    COMportInstance->Disconnect();
    return 0;
}

