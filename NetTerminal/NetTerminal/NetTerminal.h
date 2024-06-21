#ifndef NETTERMINAL_H
#define NETTERMINAL_H

#include <iostream>
#include <string>

#ifdef _WIN32
#include "../NetSocket/NetSocket.h"
#else
#include "NetSocket.h"
#endif

class NetTerminal {
private:
    NetSocketServer *_net_server = nullptr;
    NetSocketClient *_net_client = nullptr;
    
    template<class T>
    void PtrSafeRelease(T **ptr);
    template<class T>
    void ArrSafeRelease(T **ptr, const size_t &size);
    
    void Input(char *input, int size);
    
    void SmartSend(NetSocket *socket, char *buffer, const size_t &buffer_size);
    int SmartRecieve(NetSocket *socket, char **buffer);
    
    unsigned long long GetFileData(char **buffer);
    
public:
    NetTerminal(const char *ip = nullptr, unsigned short port = 0u);
    ~NetTerminal();
    
    void WaitingCommands();
    
    void RunCommands();
};

template<class T>
inline void NetTerminal::PtrSafeRelease(T **ptr) {
    if (*ptr) {
        //memset((void*)*ptr, 0, sizeof(T));
        delete *ptr;
        *ptr = nullptr;
    }
}

template<class T>
inline void NetTerminal::ArrSafeRelease(T **ptr, const size_t &size) {
    if (*ptr) {
        memset((void *)*ptr, 0, size);
        delete[] *ptr;
        *ptr = nullptr;
    }
}

#endif /* NETTERMINAL_H */
