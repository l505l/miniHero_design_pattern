
/*
#pragma once
#ifndef __NET_H__
#define __NET_H__

#include<WinSock2.h>
#include<cocos2d.h>

class Net
{
private:
    SOCKET _server;
    SOCKET _client;
    bool _isServer;
    bool _isListen;
    bool _isConnect;

    void setNonBlock(SOCKET sock)
    {
        u_long arg = 1;
        ioctlsocket(sock, FIONBIO, &arg);
    }

public:
    Net() 
    {
        _server = INVALID_SOCKET;
        _client = INVALID_SOCKET;
        _isServer = false;
        _isListen = false;
        _isConnect = false;
    }

    // 析构函数
    ~Net() 
    {
        closeNet();
    }

    void closeNet()
    {
        closesocket(_client);
        closesocket(_server);
        CCLOG("close success!");
    }

    // Getter和Setter方法
    bool getIsServer() const { return _isServer; }
    void setIsServer(bool value) { _isServer = value; }
    
    bool getIsListen() const { return _isListen; }
    void setIsListen(bool value) { _isListen = value; }
    
    bool getIsConnect() const { return _isConnect; }
    void setIsConnect(bool value) { _isConnect = value; }

    bool StartServer(unsigned short port);
    bool Accept();
    bool Connect(unsigned short port, const char* ipaddr);
    int Recv(void* buf, int len);
    int Send(char* buf, int len);
};
*/

#pragma once
#ifndef __NET_H__
#define __NET_H__

#include<WinSock2.h>
#include<cocos2d.h>
#include <mutex>

/**
 * @brief 网络通信管理类 - 单例模式实现
 * 负责管理TCP网络连接，支持客户端和服务器端模式
 */
class Net
{
private:   
	// 单例模式的静态实例指针
	static Net* _instance;
	// 用于线程同步的互斥锁
	static std::mutex _mutex;
	
	// 套接字句柄
	SOCKET _server;    // 服务器套接字
	SOCKET _client;    // 客户端套接字
	
	// 网络状态标志
	bool _isServer;    // 是否为服务器模式
	bool _isListen;    // 是否正在监听
	bool _isConnect;   // 是否已连接

	/**
	 * @brief 私有构造函数，防止外部直接创建实例
	 * 初始化所有成员变量为默认值
	 */
	Net() : _server(INVALID_SOCKET), _client(INVALID_SOCKET),
			_isServer(false), _isListen(false), _isConnect(false) {}
	
	/**
	 * @brief 禁用拷贝构造和赋值操作
	 * 确保单例的唯一性
	 */
	Net(const Net&) = delete;
	Net& operator=(const Net&) = delete;

	/**
	 * @brief 关闭网络连接
	 * 清理服务器和客户端套接字
	 */
	void closeNet()
	{
		closesocket(_client);
		closesocket(_server);
		CCLOG("close success!");
	}

	/**
	 * @brief 设置套接字为非阻塞模式
	 * @param sock 要设置的套接字
	 */
	void setNonBlock(SOCKET sock)
	{
		u_long arg = 1;
		ioctlsocket(sock, FIONBIO, &arg);
	}

public:
	/**
	 * @brief 获取单例实例
	 * @return Net* 返回Net类的唯一实例
	 * 线程安全的实现，使用互斥锁保护
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
	 * @brief 析构函数
	 * 负责清理网络资源，关闭套接字
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
	 * @brief 启动服务器
	 * @param port 监听端口号
	 * @return bool 启动是否成功
	 */
	bool StartServer(unsigned short port);

	/**
	 * @brief 接受客户端连接
	 * @return bool 接受连接是否成功
	 */
	bool Accept();

	/**
	 * @brief 连接到服务器
	 * @param port 服务器端口
	 * @param ipaddr 服务器IP地址
	 * @return bool 连接是否成功
	 */
	bool Connect(unsigned short port, const char* ipaddr);

	/**
	 * @brief 接收数据
	 * @param buf 接收缓冲区
	 * @param len 缓冲区长度
	 * @return int 接收到的数据长度，-1表示失败
	 */
	int Recv(void* buf, int len);

	/**
	 * @brief 发送数据
	 * @param buf 发送缓冲区
	 * @param len 要发送的数据长度
	 * @return int 发送的数据长度，-1表示失败
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