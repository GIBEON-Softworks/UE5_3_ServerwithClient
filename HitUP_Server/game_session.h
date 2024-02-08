#pragma once
#include <string>
#include "tcp_session.h"

class RoomWorker;
class GameSession : public TcpSession
{
public:
	GameSession()
		:room_(nullptr), session_id_(0) 
	{
	}

	~GameSession() {}

public:
	// 이름
	inline std::string GetName() {return name_; }
	inline void SetName(char* name)	{ name_ = name;	}

	// 세션 아이디
	inline uint64_t GetSessionId() { return session_id_; }
	inline void SetSessionId(uint64_t session_id) {	session_id_ = session_id; }

	// 방 정보
	inline void SetRoom(RoomWorker* room) { room_ = room; }
	inline RoomWorker* GetRoom() { return room_; }

	// 마지막 패킷 타임
	inline void SetLastTime(uint64_t time) { last_packet_time_ = time; }
	inline uint64_t GetLastTime() { return last_packet_time_; }

private:
	std::string name_;
	uint64_t session_id_;
	RoomWorker* room_;
	uint64_t last_packet_time_;
};

