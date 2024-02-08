#pragma once
#include "worker_thread.h"
#include "game_session.h"
#include "game_packet.h"
#include <set>

class RoomWorker: public WorkerThread
{
public:
	RoomWorker(PACKET_FUNC callback, uint16_t thread_count, uint16_t interval);

	void Broadcast(BasePacket* packet, GameSession* except_session = nullptr);

public:
	inline void AddSession(GameSession* session)
	{
		session_list_.insert(session);
	}

	void DeleteSession(GameSession* session);

	inline size_t GetSessionCount()
	{
		return session_list_.size();
	}


	inline std::set<GameSession*> GetSessionList()
	{
		return session_list_;
	}

private:
	virtual void WorkerAfter();


private:
	std::set<GameSession*> session_list_;
};

