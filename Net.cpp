/*
#include "Net.h"
Net* Net::_theOne = NULL;

bool Net::StartServer(unsigned short port)
{
	//����socket
	_server = socket(AF_INET, SOCK_STREAM, 0);
	if (_server == INVALID_SOCKET)
	{
		int err = WSAGetLastError();
		CCLOG("Socket creat error %d", err);
		return false;
	}
	setNonBlock(_server);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	int ret = bind(_server, (struct sockaddr*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		CCLOG("bind sockett error!");
		closesocket(_server);
		return false;
	}
	listen(_server, 10);
	_isServer = true;
	_isListen = true;
	_isConnect = false;
	return true;
}

bool Net::Accept()
{
	if (!_isServer || !_isListen || _isConnect) return false;
	_client = accept(_server, NULL, NULL);
	if (_client == INVALID_SOCKET)
		return false;
	setNonBlock(_client);
	_isConnect = true;
	return true;
}

bool Net::Connect(unsigned short port, const char* ipaddr)
{
	//����socket
	_client = socket(AF_INET, SOCK_STREAM, 0);
	if (_client == INVALID_SOCKET)
	{
		CCLOG("err");
		return false;
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = inet_addr(ipaddr);

	int ret = connect(_client, (struct sockaddr*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		CCLOG("connect error!");
		closesocket(_client);
		_client = INVALID_SOCKET;
		return false;
	}

	setNonBlock(_client);
	_isServer = false;
	_isListen = false;
	_isConnect = true;
	return true;
}

//�ͻ��˺ͷ���˹��ýӿ�
int Net::Recv(void* buf, int len)
{
	if (!_isConnect) return-1;//24 04

	int ret = recv(_client, (char*)buf, len, 0);
	return ret;
}

int Net::Send(char* buf, int len)
{
	if (!_isConnect) return -1;
	return send(_client, buf, len, 0);
}
*/


#include "Net.h"

// 初始化静态成员
Net* Net::_instance = nullptr;
std::mutex Net::_mutex;

bool Net::StartServer(unsigned short port)
{
	// 创建服务器套接字
	_server = socket(AF_INET, SOCK_STREAM, 0);
	if (_server == INVALID_SOCKET)
	{
		int err = WSAGetLastError();
		CCLOG("Socket create error %d", err);
		return false;
	}
	// 设置非阻塞模式
	setNonBlock(_server);

	// 设置服务器地址结构
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	// 绑定地址和端口
	int ret = bind(_server, (struct sockaddr*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		CCLOG("bind socket error!");
		closesocket(_server);
		return false;
	}
	// 开始监听
	listen(_server, 10);
	_isServer = true;
	_isListen = true;
	_isConnect = false;
	return true;
}

bool Net::Accept()
{
	// 检查状态是否允许接受连接
	if (!_isServer || !_isListen || _isConnect) return false;
	
	// 接受客户端连接
	_client = accept(_server, NULL, NULL);
	if (_client == INVALID_SOCKET)
		return false;
		
	// 设置客户端套接字为非阻塞模式
	setNonBlock(_client);
	_isConnect = true;
	return true;
}

bool Net::Connect(unsigned short port, const char* ipaddr)
{
	// 创建客户端套接字
	_client = socket(AF_INET, SOCK_STREAM, 0);
	if (_client == INVALID_SOCKET)
	{
		CCLOG("err");
		return false;
	}
	
	// 设置服务器地址结构
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = inet_addr(ipaddr);

	// 连接到服务器
	int ret = connect(_client, (struct sockaddr*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		CCLOG("connect error!");
		closesocket(_client);
		_client = INVALID_SOCKET;
		return false;
	}

	// 设置非阻塞模式并更新状态
	setNonBlock(_client);
	_isServer = false;
	_isListen = false;
	_isConnect = true;
	return true;
}

int Net::Recv(void* buf, int len)
{
	// 检查连接状态
	if (!_isConnect) return -1;

	// 接收数据
	int ret = recv(_client, (char*)buf, len, 0);
	return ret;
}

int Net::Send(char* buf, int len)
{
	// 检查连接状态
	if (!_isConnect) return -1;
	
	// 发送数据
	return send(_client, buf, len, 0);
}