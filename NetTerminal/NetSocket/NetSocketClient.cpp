#include "NetSocketClient.h"

NetSocketClient::NetSocketClient(const char *ip, const unsigned short &port) {
	//CloseSocket(_conn_socket);

	//memset(&_conn_info, 0, _conn_info_size);
	_conn_info.sin_family = AF_INET;
	_conn_info.sin_port = htons(port);
	inet_pton(AF_INET, ip, &_conn_info.sin_addr);

	// Create a SOCKET for connecting to server
	if ((_conn_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		ThrowSockExcept("socket failed");
	}

	// Connect to server.
	if (connect(_conn_socket, (const sockaddr *)&_conn_info, _conn_info_size) < 0) {
		ThrowSockExcept("socket connection failed");
	}
}
