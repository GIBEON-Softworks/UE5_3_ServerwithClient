#pragma once

#ifndef _WINSOCK2API_
#include <WinSock2.h>
#include <Windows.h>
#endif

#include <ws2tcpip.h>

#include <vector>
#include <thread>
#include <map>
#include <mutex>
#include "packet_handler.h"
#include "thread_pool.h"

class TcpSession;
class ThreadPool;

class IocpServer
{
public:
	IocpServer();
	~IocpServer();

public:
	virtual void Init() {};
	virtual void ExitSession(TcpSession* exitSession) {};
	void Start(uint16_t thread_count, uint16_t port);

public:
	virtual TcpSession* CreateSession();

public:
	template<typename T>
	void AddPacket(PACKET_FUNC callback, size_t pool_count);

	void Broadcast(BasePacket* packet);

private:
	int  InitSocket();
	void InitIocp();
	void Worker(uint16_t thread_index);
	void Run();
	void CloseSession(TcpSession*);


protected:
	std::map<SOCKET, TcpSession*>	session_map_;
	ThreadSafeQueue<TcpSession*>	session_list_;
	PacketHandler* packet_handler_;

private:
	uint16_t				port_;
	SOCKET					listen_socket_;
	HANDLE					iocp_;
	ThreadPool* thread_pool_;
	uint16_t				thread_count_;
	std::mutex				session_lock_;
};

template<typename T>
void IocpServer::AddPacket(PACKET_FUNC callback, size_t pool_count)
{
	for (uint16_t i = 0; i < thread_count_; i++)
	{
		packet_handler_[i].AddPacket<T>(callback, pool_count);
	}
}
