#ifndef NET_SOCKET_SERVER_H
#define NET_SOCKET_SERVER_H

#include "NetSocketBase.h"

class NetSocketServer : public NetSocket {
private:
	int _listen_socket = -1;
	sockaddr_in _sock_info = { 0 };

public:
	NetSocketServer(const unsigned short &port);
	virtual ~NetSocketServer() override;

	int AcceptConnection();
};

#endif/* NET_SOCKET_SERVER_H */
