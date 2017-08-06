
/* 
 * File:   tty.h
 * Author: some_name
 *
 * Created on 3 июля 2017 г., 16:22
 */

#ifndef TTY_H
#define TTY_H
 //uncomment this line if not mingw (os_defines.h)
 //#define NOMINMAX 
 //иначе API windows определит макросы min и max, конфликтующие с std::max и std::min в vector

 #include <windows.h>
 #include <vector>
 #include <string>
 #include "stdint.h"
 using namespace std;

 typedef enum {NONE, ODD, EVEN, MARK, SPACE} PARITY;
 
class tty {
public:
    tty();
    virtual ~tty();
 	bool IsOK() const;	
        //TODO: define also handshaking. It is not used anyway.
 	void Connect(string* port, uint32_t baudrate, uint8_t dataBits, uint8_t stopBits, PARITY parityInstance);
 	void Disconnect();
 	virtual void Write(const vector<unsigned char>& data);
        virtual void WriteByte(const uint8_t singleByte);
        virtual uint8_t ReadByte();
 	virtual void Read(vector<unsigned char>& data);	
 	HANDLE m_Handle;
private:

};

class TTYException {
public:
    TTYException() {
        exception_cause = new string();
        exception_cause = new string("Unkonown Source");
    }
    string* exception_cause;
    TTYException(string* expl) {
        exception_cause = new string(*expl);
    }
};

#endif /* TTY_H */

