#include "room_worker.h"
#include "game_session.h"
#include "packet_handler.h"
#include "game_packet.h"
#include <vector>

RoomWorker::RoomWorker(PACKET_FUNC callback, uint16_t thread_count, uint16_t interval)
	:WorkerThread(callback, thread_count, interval)
{
	
}

void RoomWorker::Broadcast(BasePacket* packet, GameSession* except_session)
{
	for (auto iter : session_list_)
	{
		if (iter == except_session) continue;
		iter->CallSend(packet, packet->packet_len);
	}
}

void RoomWorker::WorkerAfter()
{
	int64_t now = time(nullptr);
	std::vector<GameSession*> del_session;

	// 해당 룸에 패킷 보낸지 3초 이상 지났으면 out
	for (auto session : session_list_)
	{
		int64_t interval = now - session->GetLastTime();

		if (interval > 3)
		{
			del_session.push_back(session);
		}
	}

	for (auto session : del_session)
	{
		DeleteSession(session);
	}
}


void RoomWorker::DeleteSession(GameSession* session)
{
	session_list_.erase(session);
	BroadOutRoomPacket packet;
	packet.session_id = session->GetSessionId();

	Broadcast(&packet);

}