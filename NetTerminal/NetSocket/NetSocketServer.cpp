#include "NetSocketServer.h"

NetSocketServer::NetSocketServer(const unsigned short &port) {
	//memset(&_sock_info, 0, sizeof(_sock_info));
	_sock_info.sin_family = AF_INET;
	_sock_info.sin_addr.s_addr = htonl(INADDR_ANY);
	_sock_info.sin_port = htons(port);

	// Create a SOCKET for the server to listen for client connections
	if ((_listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		ThrowSockExcept("socket failed");
	}

	// Setup the TCP listening socket
	if (bind(_listen_socket, (const sockaddr *)&_sock_info, sizeof(_sock_info)) < 0) {
		ThrowSockExcept("bind failed");
	}

	if (listen(_listen_socket, SOMAXCONN) < 0) {
		ThrowSockExcept("listen failed");
	}
}

NetSocketServer::~NetSocketServer() {
	CloseSocket(_listen_socket);
}

int NetSocketServer::AcceptConnection() {
	memset(&_conn_info, 0, sizeof(_conn_info));
	CloseSocket(_conn_socket);

	_conn_socket = accept(_listen_socket, (sockaddr *)&_conn_info, &_conn_info_size);
	if (_conn_socket < 0) {
		ThrowSockExcept("accept failed");
	}

	return (int)_conn_socket;
}
