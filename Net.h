/* Refactored using Singleton Pattern */
#pragma once
#ifndef __NET_H__
#define __NET_H__

#include<WinSock2.h>
#include<cocos2d.h>
#include <mutex>

/**
 * @brief Network Communication Manager - Singleton Pattern Implementation
 * Manages TCP network connections, supporting both client and server modes
 */
class Net
{
private:   
	// Singleton instance pointer
	static Net* _instance;
	// Mutex for thread synchronization
	static std::mutex _mutex;
	
	// Socket handles
	SOCKET _server;    // Server socket
	SOCKET _client;    // Client socket
	
	// Network status flags
	bool _isServer;    // Whether in server mode
	bool _isListen;    // Whether listening
	bool _isConnect;   // Whether connected

	/**
	 * @brief Private constructor to prevent direct instantiation (Singleton Pattern)
	 * Initializes all member variables to default values
	 */
	Net() : _server(INVALID_SOCKET), _client(INVALID_SOCKET),
			_isServer(false), _isListen(false), _isConnect(false) {}
	
	/**
	 * @brief Delete copy constructor and assignment operator (Singleton Pattern)
	 * Ensures single instance
	 */
	Net(const Net&) = delete;
	Net& operator=(const Net&) = delete;

	/**
	 * @brief Close network connections
	 * Clean up server and client sockets
	 */
	void closeNet()
	{
		closesocket(_client);
		closesocket(_server);
		CCLOG("close success!");
	}

	/**
	 * @brief Set socket to non-blocking mode
	 * @param sock Socket to be configured
	 */
	void setNonBlock(SOCKET sock)
	{
		u_long arg = 1;
		ioctlsocket(sock, FIONBIO, &arg);
	}

public:
	/**
	 * @brief Get singleton instance (Thread-safe implementation)
	 * @return Net* Returns the single instance of Net class
	 * Uses mutex lock for thread safety
	 */
	static Net* getInstance()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (_instance == nullptr) {
			_instance = new Net();
		}
		return _instance;
	}

	/**
	 * @brief Destructor
	 * Responsible for cleaning up network resources and closing sockets
	 */
	~Net() {
		if (_server != INVALID_SOCKET) {
			closesocket(_server);
		}
		if (_client != INVALID_SOCKET) {
			closesocket(_client);
		}
	}

	/**
	 * @brief Start server
	 * @param port Port number to listen on
	 * @return bool Success status of server start
	 */
	bool StartServer(unsigned short port);

	/**
	 * @brief Accept client connection
	 * @return bool Success status of connection acceptance
	 */
	bool Accept();

	/**
	 * @brief Connect to server
	 * @param port Server port
	 * @param ipaddr Server IP address
	 * @return bool Success status of connection
	 */
	bool Connect(unsigned short port, const char* ipaddr);

	/**
	 * @brief Receive data
	 * @param buf Receive buffer
	 * @param len Buffer length
	 * @return int Length of received data, -1 if failed
	 */
	int Recv(void* buf, int len);

	/**
	 * @brief Send data
	 * @param buf Send buffer
	 * @param len Length of data to send
	 * @return int Length of sent data, -1 if failed
	 */
	int Send(char* buf, int len);
};

#endif

class Net {
public:
	Net() {
		_server = INVALID_SOCKET;
		_client = INVALID_SOCKET;
		_isServer = false;
		_isListen = false;
		_isConnect = false;
	}
};