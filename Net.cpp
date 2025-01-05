/* Refactored by Singleton Pattern */
#include "Net.h"

// Initialize static members for singleton pattern
Net* Net::_instance = nullptr;
std::mutex Net::_mutex;

bool Net::StartServer(unsigned short port)
{
	// Create server socket
	_server = socket(AF_INET, SOCK_STREAM, 0);
	if (_server == INVALID_SOCKET)
	{
		int err = WSAGetLastError();
		CCLOG("Socket create error %d", err);
		return false;
	}
	// Set non-blocking mode
	setNonBlock(_server);

	// Configure server address structure
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	// Bind address and port
	int ret = bind(_server, (struct sockaddr*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		CCLOG("bind socket error!");
		closesocket(_server);
		return false;
	}
	// Start listening for connections
	listen(_server, 10);
	_isServer = true;
	_isListen = true;
	_isConnect = false;
	return true;
}

bool Net::Accept()
{
	// Check if singleton instance is in valid state to accept connections
	if (!_isServer || !_isListen || _isConnect) return false;
	
	// Accept client connection
	_client = accept(_server, NULL, NULL);
	if (_client == INVALID_SOCKET)
		return false;
		
	// Set client socket to non-blocking mode
	setNonBlock(_client);
	_isConnect = true;
	return true;
}

bool Net::Connect(unsigned short port, const char* ipaddr)
{
	// Create client socket for singleton instance
	_client = socket(AF_INET, SOCK_STREAM, 0);
	if (_client == INVALID_SOCKET)
	{
		CCLOG("err");
		return false;
	}
	
	// Configure server address structure
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = inet_addr(ipaddr);

	// Connect to server
	int ret = connect(_client, (struct sockaddr*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		CCLOG("connect error!");
		closesocket(_client);
		_client = INVALID_SOCKET;
		return false;
	}

	// Set non-blocking mode and update singleton state
	setNonBlock(_client);
	_isServer = false;
	_isListen = false;
	_isConnect = true;
	return true;
}

int Net::Recv(void* buf, int len)
{
	// Check if singleton instance is connected
	if (!_isConnect) return -1;

	// Receive data through singleton's client socket
	int ret = recv(_client, (char*)buf, len, 0);
	return ret;
}

int Net::Send(char* buf, int len)
{
	// Check if singleton instance is connected
	if (!_isConnect) return -1;
	
	// Send data through singleton's client socket
	return send(_client, buf, len, 0);
}