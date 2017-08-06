/* 
 * File:   tty.cpp
 * Author: some_name and wikipedia
 * operate with com port.
 * (1) https://ru.wikibooks.org/wiki/COM-%D0%BF%D0%BE%D1%80%D1%82_%D0%B2_Windows_(%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5)
 * (2) http://blablacode.ru/programmirovanie/392
 * (3) http://xanthium.in/Serial-Port-Programming-using-Win32-API
 * (4) https://github.com/xanthium-enterprises/Serial-Programming-Win32API-C
 * (5) Configuring a Communications Resource: https://msdn.microsoft.com/en-us/library/windows/desktop/aa363201(v=vs.85).aspx
 * (6) Monitoring Communications Events: https://msdn.microsoft.com/en-us/library/windows/desktop/aa363424(v=vs.85).aspx
 * (7) Com port naming: https://support.microsoft.com/en-us/help/115831/howto-specify-serial-ports-larger-than-com9
 * Created on 3 июля 2017 г., 16:22
 */

#include "tty.h"
#include <iostream>
#include <assert.h>
#include <windows.h>

static int TIMEOUT = 1000;

tty::tty() {
    m_Handle = INVALID_HANDLE_VALUE;
}

void tty::Connect(string* port, uint32_t baudrate, uint8_t dataBits, uint8_t stopBits, PARITY parityInstance) {
    Disconnect();
    
    m_Handle = CreateFile(
 		port->c_str(),             // Name of the Port to be Opened: Example: "\\\\.\\COM10"
 		GENERIC_READ | GENERIC_WRITE, // Read/Write Access
 		0,                            // No Sharing, ports cant be shared
 		NULL,                         // No Security
 		OPEN_EXISTING,                // Open existing port only
 		FILE_FLAG_OVERLAPPED,         // overlapped/nonoverlapped IO. might be worth setting to 0 (3)
                                              // or FILE_ATTRIBUTE_NORMAL (see 1) or FILE_FLAG_OVERLAPPED (see 6)
 		NULL);                        //  hTemplate must be NULL for comm devices
    
            if(m_Handle == INVALID_HANDLE_VALUE) {
                
                char errcode [30];
                uint8_t lngth = sprintf(errcode, "Error opening port: %05d", GetLastError());
                throw TTYException( new string(errcode, lngth) );
                        
            }
        ////Configure Windows to Monitor the serial device for Character Reception
        SetCommMask(m_Handle, EV_RXCHAR);
        //https://msdn.microsoft.com/ru-ru/library/windows/desktop/aa363439(v=vs.85).aspx
        //The dwInQueue and dwOutQueue parameters specify the recommended sizes for the internal buffers used by the driver for the specified device.
 	SetupComm(m_Handle, 1500, 1500);
        /*------------------------------------ Setting Timeouts --------------------------------------------------*/
        //see https://msdn.microsoft.com/ru-ru/library/windows/desktop/aa363190(v=vs.85).aspx
 	COMMTIMEOUTS CommTimeOuts;
            //The maximum time allowed to elapse before the arrival of the next byte on the communications line, in milliseconds.
 	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF; 
 	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;  
 	CommTimeOuts.ReadTotalTimeoutConstant = TIMEOUT; 
 	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
 	CommTimeOuts.WriteTotalTimeoutConstant = TIMEOUT;
        /*
        CommTimeOuts.ReadIntervalTimeout         = 50;
	CommTimeOuts.ReadTotalTimeoutConstant    = 50;
	CommTimeOuts.ReadTotalTimeoutMultiplier  = 10;
	CommTimeOuts.WriteTotalTimeoutConstant   = 50;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 10;
         */
        
        if(!SetCommTimeouts(m_Handle, &CommTimeOuts)) {
 		CloseHandle(m_Handle);
                m_Handle = INVALID_HANDLE_VALUE;
 		throw TTYException();
 	}	
 	DCB ComDCM;	
 	memset(&ComDCM,0,sizeof(ComDCM));
 	ComDCM.DCBlength = sizeof(DCB);
 	GetCommState(m_Handle, &ComDCM);
 	ComDCM.BaudRate = DWORD(baudrate);
 	ComDCM.ByteSize = 8;
 	ComDCM.Parity = NOPARITY;
 	ComDCM.StopBits = ONESTOPBIT;
 	ComDCM.fAbortOnError = TRUE;
 	ComDCM.fDtrControl = DTR_CONTROL_DISABLE;
 	ComDCM.fRtsControl = RTS_CONTROL_DISABLE;
 	ComDCM.fBinary = TRUE;
 	ComDCM.fParity = FALSE;
 	ComDCM.fInX = FALSE;
        ComDCM.fOutX = FALSE;
 	ComDCM.XonChar = 0;
 	ComDCM.XoffChar = (unsigned char)0xFF;
 	ComDCM.fErrorChar = FALSE;
 	ComDCM.fNull = FALSE;
 	ComDCM.fOutxCtsFlow = FALSE;
 	ComDCM.fOutxDsrFlow = FALSE;
 	ComDCM.XonLim = 128;
 	ComDCM.XoffLim = 128;
        
        if(!SetCommState(m_Handle, &ComDCM)) {
                CloseHandle(m_Handle);
                m_Handle = INVALID_HANDLE_VALUE;
                throw TTYException();
 	}
        
}

void tty::Disconnect() {    
    if(m_Handle != INVALID_HANDLE_VALUE) {
 	   CloseHandle(m_Handle);
           m_Handle = INVALID_HANDLE_VALUE;
    }    
}

void tty::Write(const vector<unsigned char>& data) {

 	if(m_Handle == INVALID_HANDLE_VALUE) {
 		throw TTYException();
 	}

 	DWORD feedback;
 	if(!WriteFile(m_Handle, &data[0], (DWORD)data.size(), &feedback, 0) || feedback != (DWORD)data.size()) {
 		CloseHandle(m_Handle);
 		m_Handle = INVALID_HANDLE_VALUE;
 		throw TTYException();
 	}

 	// In some cases it's worth uncommenting
 	//FlushFileBuffers(m_Handle);
}

void tty::WriteByte(const uint8_t singleByte) {

}
uint8_t tty::ReadByte() {

}
void tty::Read(vector<unsigned char>& data) {
    
}

tty::~tty() {
    Disconnect();
}

