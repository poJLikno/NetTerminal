#ifndef SOCKET_BASE_H
#define SOCKET_BASE_H

#include <string>

#ifdef _WIN32
// first sockets-h, then windows-h
#include <ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")
#else
// libs for unix
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//#define INVALID_SOCKET ~0// -1
//#define SOCKET_ERROR -1
//#define SD_SEND -1
#endif/* Check os */

class NetSocket {
protected:
#ifdef _WIN32
	static unsigned int _win_sock_users;
	WSADATA _wsa_data = { 0 };
#endif/* Window sockets init structures */

	int _conn_socket = -1;
	sockaddr_in _conn_info = { 0 };
	socklen_t _conn_info_size = 0;
	
	virtual void CloseSocket(int &sock) final;
	virtual void ThrowSockExcept(const char *error_message) final;

	NetSocket();

public:
	virtual ~NetSocket();

	virtual void ShutdownConnection() final;

	// Max buffer size 65'535 bytes
	virtual int Recieve(char *buffer, const size_t &size) final;
	virtual int Send(char *buffer, const size_t &size) final;
};

#endif/* SOCKET_BASE_H */
