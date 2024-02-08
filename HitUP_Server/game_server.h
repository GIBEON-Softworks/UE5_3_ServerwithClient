#pragma once
#include <atomic>
#include <vector>
#include "iocp_server.h"
#include "game_session.h"

struct BasePacket;
class RoomWorker;

class GameServer: public IocpServer
{
public:
	GameServer();
	~GameServer();

public:
	virtual void Init();

public:
	virtual TcpSession* CreateSession()
	{
		return new GameSession;
	}

	virtual void ExitSession(TcpSession* exitSession);

private:
	void WorkerPacketCallback(Packet& packet);
	void WorkerProcessCallback(Packet& packet);

	void Login(Packet& packet);
	void JoinRoom(Packet& packet);
	void Move(Packet& packet);

private:
	std::vector<RoomWorker*> room_list_;

	std::atomic<uint64_t> session_inc_;
};

