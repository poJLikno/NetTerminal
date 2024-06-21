#include "NetSocketBase.h"

#ifdef _WIN32
unsigned int NetSocket::_win_sock_users = 0u;
#endif

void NetSocket::ThrowSockExcept(const char *error_message) {
#ifdef _WIN32
	throw error_message + std::string(": ") + std::to_string(WSAGetLastError());
#else
	throw std::string(error_message);
#endif
}

void NetSocket::CloseSocket(int &sock) {
	if (sock < 0) return;
#ifdef _WIN32
	closesocket((SOCKET)sock);
#else
	close((int)sock);
#endif
	sock = -1;
}

NetSocket::NetSocket() {
#ifdef _WIN32
	// Initialize Winsock
	if (!_win_sock_users) {
		int result = WSAStartup(MAKEWORD(2, 2), &_wsa_data);
		if (result) {
			throw std::string("WSAStartup failed with error: " + result);// Windows Only exception
		}
	}
	++_win_sock_users;
#endif

	_conn_info_size = sizeof(_conn_info);

	//_socket_state = Init_State;
}

NetSocket::~NetSocket() {
	// Clean up
	CloseSocket(_conn_socket);
	//CloseSocket(_listen_socket);
	
#ifdef _WIN32
	--_win_sock_users;
	if (_win_sock_users == 0) {
		WSACleanup();
	}
#endif

	//_socket_state = None_State;
}

void NetSocket::ShutdownConnection() {
	// shutdown the connection since we're done
	if (shutdown(_conn_socket, 1/*SD_SEND*/) < 0) {
		ThrowSockExcept("shutdown failed");
	}
}

// If return 0, close connection
int NetSocket::Recieve(char *buffer, const size_t &size) {
	int result = 0;
	if ((result = (int)recv(_conn_socket, buffer, size, 0)) < 0) {
		ThrowSockExcept("recv failed");
	}

	return result;
}

int NetSocket::Send(char *buffer, const size_t &size) {
	int result = 0;
	if ((result = (int)send(_conn_socket, buffer, size, 0)) < 0) {
		ThrowSockExcept("send failed");
	}

	return result;
}
