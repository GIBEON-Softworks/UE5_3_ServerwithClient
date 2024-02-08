#pragma once
#include <functional>
#include <unordered_map>
#include "thread_safe_queue.h"

class WorkerThread;

#pragma pack(push, 1)
struct BasePacket
{
	uint16_t packet_len;
	uint16_t packet_id;
};
#pragma pack(pop)


#define PACKET_HEADER_SIZE sizeof(BasePacket)
#define PACKET_FUNC_C(f) std::bind(&f, this, std::placeholders::_1)


class Packet;
class TcpSession;

typedef std::function<void(Packet&)> PACKET_FUNC;

class PacketHandler
{
public:
	PacketHandler();
	~PacketHandler();

public:
	template<typename T>
	bool AddPacket(PACKET_FUNC callback, size_t pool_count);
	bool ProcessPacket(TcpSession* tcp_session);
	void RestorePacket(BasePacket* packet);

private:
	void Release();

private:
	std::unordered_map<uint16_t, PACKET_FUNC> callback_list_;
	std::unordered_map<uint16_t, ThreadSafeQueue<BasePacket*>*> packet_pool_;
};


template<typename T>
bool PacketHandler::AddPacket(PACKET_FUNC callback, size_t pool_count)
{
	T packet;

	// �ߺ� id
	if (callback_list_.find(packet.packet_id) != callback_list_.end())
		return false;


	callback_list_.insert({ packet.packet_id , callback });


	auto queue = new ThreadSafeQueue<BasePacket*>;

	for (size_t i = 0; i < pool_count; i++)
	{
		queue->Push(new T);
	}

	packet_pool_.insert({ packet.packet_id , queue });

	return true;
}


class Packet
{
public:
	Packet(PacketHandler* handler, TcpSession* session, BasePacket* packet)
		:handler_(handler),
		session_(session),
		packet_(packet)
	{
	}

	void Release()
	{
		if (handler_ == nullptr) return;

		handler_->RestorePacket(packet_);

		session_ = nullptr;
		packet_ = nullptr;
		handler_ = nullptr;
	}

public:
	TcpSession* session_;
	BasePacket* packet_;

private:
	PacketHandler* handler_;
};


