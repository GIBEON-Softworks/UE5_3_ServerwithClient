// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "HAL/Runnable.h"
#include "ThreadSafeQueue.h"
#include <unordered_map>

class StreamBuffer;


#pragma pack(push, 1)
struct BasePacket
{
	uint16 packet_len;
	uint16 packet_id;
};
#pragma pack(pop)

struct Packet
{
	ThreadSafeQueue<BasePacket*>* pool;
	BasePacket* packet;

	void Relese()
	{
		pool->Push(packet);
	}
};

class HITUP_API Connector : public FRunnable
{
public:
	Connector();
	~Connector();

public:
	bool Connection(FString host, int32 port, uint16 buffer_size = 10240);

public: // 스레드 인터페이스
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;

public:
	template<typename T>
	bool AddPacket(uint16 pool_count);
	bool GeneratedPacket();
	void Send(BasePacket* packet);

	void GetMessage(TArray<Packet>& packets)
	{
		MessageQueue.DrainQueue(packets);
	}

private:
	FSocket* Socket;
	StreamBuffer* Buffer;
	FRunnableThread* Thread;

	ThreadSafeQueue<Packet> MessageQueue;
	TMap<uint16, ThreadSafeQueue<BasePacket*>*> PacketPool;
};

template<typename T>
bool Connector::AddPacket(uint16 pool_count)
{
	T packet;

	auto queue = new ThreadSafeQueue<BasePacket*>;

	for (size_t i = 0; i < pool_count; i++)
	{
		queue->Push(new T);
	}

	PacketPool.Add(packet.packet_id, queue);

	return true;
}