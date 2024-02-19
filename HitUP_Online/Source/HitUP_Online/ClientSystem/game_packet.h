#pragma once
#include "Connector.h"


enum GAME_PACKET
{
	REQ_LOGIN,
	RES_LOGIN,
	REQ_JOIN_ROOM,
	RES_JOIN_ROOM,
	BROAD_JOIN_ROOM,
	BROAD_OUT_ROOM,
	REQ_MOVE,
	RES_MOVE,
	REQ_ANIMATION, // 새로운 패킷 유형 추가: 애니메이션 요청
	RES_ANIMATION  // 새로운 패킷 유형 추가: 애니메이션 응답
};

#pragma pack(push, 1)
struct ReqLoginPacket : public BasePacket
{
	ReqLoginPacket()
	{
		packet_id = REQ_LOGIN;
		packet_len = sizeof(ReqLoginPacket);
	}
	char name[20];
};

struct ResLoginPacket : public BasePacket
{
	ResLoginPacket()
	{
		packet_id = RES_LOGIN;
		packet_len = sizeof(ResLoginPacket);
	}

	uint64_t session_id;
};


// 애니메이션 요청 패킷
struct ReqAnimationPacket : public BasePacket
{
	ReqAnimationPacket()
	{
		packet_id = REQ_ANIMATION;
		packet_len = sizeof(ReqAnimationPacket);
	}

	// 여기에 애니메이션 정보를 담을 필드를 추가합니다.
	// 예를 들어, 애니메이션 종류를 나타내는 enum 또는 애니메이션 이름을 담을 수 있습니다.
	// AnimationType animationType;
	// 또는
	// char animationName[50]; // 예: "Walk" 또는 "Run" 등의 문자열
};

// 애니메이션 응답 패킷
struct ResAnimationPacket : public BasePacket
{
	ResAnimationPacket()
	{
		packet_id = RES_ANIMATION;
		packet_len = sizeof(ResAnimationPacket);
	}
	// 애니메이션 정보에 대한 응답을 받을 경우 여기에 필요한 정보를 추가합니다.
	// 예를 들어, 애니메이션의 실행 여부를 나타내는 bool 값 등을 추가할 수 있습니다.
	// 이 정보를 통해 클라이언트는 서버로부터 애니메이션 정보를 동기화할 수 있습니다.
};


struct ReqJoinRoomPacket : public BasePacket
{
	ReqJoinRoomPacket()
	{
		packet_id = REQ_JOIN_ROOM;
		packet_len = sizeof(ReqJoinRoomPacket);
	}

	uint64_t room_id;
};



struct ResJoinRoomPacket : public BasePacket
{
	ResJoinRoomPacket()
	{
		packet_id = RES_JOIN_ROOM;
		packet_len = sizeof(ResJoinRoomPacket);
	}

	bool success;
	uint64_t session_list[10];
	char name[10][20];
};

struct BroadJoinRoomPacket : public BasePacket
{
	BroadJoinRoomPacket()
	{
		packet_id = BROAD_JOIN_ROOM;
		packet_len = sizeof(BroadJoinRoomPacket);
	}

	uint64_t session_id;
	char name[20];
};

struct BroadOutRoomPacket : public BasePacket
{
	BroadOutRoomPacket()
	{
		packet_id = BROAD_OUT_ROOM;
		packet_len = sizeof(BroadOutRoomPacket);
	}

	uint64_t session_id;
};


struct ReqMovePacket : public BasePacket
{
	ReqMovePacket()
	{
		packet_id = REQ_MOVE;
		packet_len = sizeof(ReqMovePacket);
	}

	float x;
	float y;
	float z;
};

struct ResMovePacket : public BasePacket
{
	ResMovePacket()
	{
		packet_id = RES_MOVE;
		packet_len = sizeof(ResMovePacket);
	}

	float x;
	float y;
	float z;
	int64_t session_id;
};

#pragma pack(pop)