#ifndef NET_SOCKET_CLIENT_H
#define NET_SOCKET_CLIENT_H

#include "NetSocketBase.h"

class NetSocketClient : public NetSocket {
public:
	NetSocketClient(const char *ip, const unsigned short &port);
	virtual ~NetSocketClient() override = default;
};

#endif/* NET_SOCKET_CLIENT_H */