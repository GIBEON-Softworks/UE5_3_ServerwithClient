#pragma once
#include "packet_handler.h"
#define MAX_ROOM_COUNT 8
#define MAX_ROOM_SESSION_COUNT 10

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
	uint64_t session_list[MAX_ROOM_SESSION_COUNT];
	char name[MAX_ROOM_SESSION_COUNT][20];
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