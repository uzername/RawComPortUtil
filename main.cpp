#include <cstdlib>
#include <cstdio>
#include <memory>
#include "tty.h"
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
    printf("%s %s", "Your line: ", portnameString->c_str() );
    try {
    COMportInstance->Connect(portnameString,9600, 8, 1, NONE);
    } catch (TTYException e) {
        printf("\n %s",e.exception_cause->c_str());
        COMportInstance->Disconnect();
        return 1;
    }
    COMportInstance->Disconnect();
    return 0;
}

