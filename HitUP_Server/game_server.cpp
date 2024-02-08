#include "game_server.h"
#include <iostream>
#include "game_session.h"
#include "packet_handler.h"
#include "room_worker.h"
#include "game_packet.h"

GameServer::GameServer()
	:session_inc_(1)
{
	uint16_t thread_count = 1;
	uint16_t interval = 50;

	for (int i = 0; i < MAX_ROOM_COUNT; i++)
	{
		auto worker_ = new RoomWorker(PACKET_FUNC_C(GameServer::WorkerProcessCallback), thread_count, interval);
		room_list_.push_back(worker_);
	}
	
}

GameServer::~GameServer()
{
	for (auto item : room_list_)
	{
		delete item;
	}

	room_list_.clear();
}

void GameServer::Init()
{
	AddPacket<ReqLoginPacket>(PACKET_FUNC_C(GameServer::Login), 100);
	AddPacket<ReqJoinRoomPacket>(PACKET_FUNC_C(GameServer::WorkerPacketCallback), 100);
	AddPacket<ReqMovePacket>(PACKET_FUNC_C(GameServer::WorkerPacketCallback), 5000);
}

void GameServer::ExitSession(TcpSession* exitSession)
{

}

void GameServer::WorkerPacketCallback(Packet& packet)
{
	RoomWorker* room = nullptr;
	auto session = reinterpret_cast<GameSession*>(packet.session_);

	// 방 입장 패킷일경우
	if (packet.packet_->packet_id == GAME_PACKET::REQ_JOIN_ROOM)
	{
		// 방 id 검사
		ReqJoinRoomPacket* room_packet = reinterpret_cast<ReqJoinRoomPacket*>(packet.packet_);
		if (room_packet->room_id >= 0 && room_packet->room_id < room_list_.size())
		{
			room = room_list_[room_packet->room_id];
			session->SetRoom(room);
		}
	}
	else
	{
		room = session->GetRoom();
	}

	if (room == nullptr) return;

	room->AddQueue(packet);
}

void GameServer::WorkerProcessCallback(Packet& packet)
{
	switch (packet.packet_->packet_id)
	{
	case GAME_PACKET::REQ_LOGIN:
		Login(packet);
		break;

	case GAME_PACKET::REQ_JOIN_ROOM:
		JoinRoom(packet);
		break;

	case GAME_PACKET::REQ_MOVE:
		Move(packet);
		break;
	}
}

void GameServer::Login(Packet& packet)
{
	GameSession* session = reinterpret_cast<GameSession*>(packet.session_);;
	ReqLoginPacket* login_packet = reinterpret_cast<ReqLoginPacket*>(packet.packet_);
	std::cout << "로그인 : " << login_packet->name << std::endl;

	// 이름및 고유 세션 키 등록
	int64_t prevId = session_inc_.fetch_add(1, std::memory_order_acq_rel);
	session->SetName(login_packet->name);
	session->SetSessionId(prevId);

	// 응답
	ResLoginPacket res;
	res.session_id = prevId;

	session->CallSend(&res, res.packet_len);

	// 필수
	packet.Release();
}

void GameServer::JoinRoom(Packet& packet)
{
	GameSession* session = reinterpret_cast<GameSession*>(packet.session_);;
	ReqJoinRoomPacket* room_packet = reinterpret_cast<ReqJoinRoomPacket*>(packet.packet_);
	RoomWorker* room = session->GetRoom();

	printf("방 참가 %lld %lld", session->GetSessionId(), room_packet->room_id);

	if (room->GetSessionCount() >= MAX_ROOM_SESSION_COUNT)
	{
		session->SetRoom(nullptr);

		// 응답
		ResJoinRoomPacket res;
		res.success = false;
		session->CallSend(&res, res.packet_len);
		return;
	}
	 
	room->AddSession(session);
	session->SetLastTime(time(nullptr));

	// 응답
	ResJoinRoomPacket res;
	res.success = true;

	auto session_list = room->GetSessionList();

	int i = 0;
	memset(res.session_list, 0, sizeof(uint64_t) * MAX_ROOM_SESSION_COUNT);
	for (auto room_session : session_list)
	{
		res.session_list[i] = room_session->GetSessionId();
		strcpy_s(res.name[i], room_session->GetName().c_str());
		i++;
	}

	session->CallSend(&res, res.packet_len);

	// 전체 알림
	BroadJoinRoomPacket broad;
	broad.session_id = session->GetSessionId();
	strcpy_s(broad.name, session->GetName().c_str());

	room->Broadcast(&broad, session);

	// 필수
	packet.Release();
}

void GameServer::Move(Packet& packet)
{
	auto session = reinterpret_cast<GameSession*>(packet.session_);;
	auto move_packet = reinterpret_cast<ReqMovePacket*>(packet.packet_);
	auto room = session->GetRoom();

	ResMovePacket res;

	res.session_id = session->GetSessionId();
	res.x = move_packet->x;
	res.y = move_packet->y;
	res.z = move_packet->z;

	session->SetLastTime(time(nullptr));
	room->Broadcast(&res, session);
}