#pragma once
#include <tuple>
#include <vector>
#include "thread_safe_queue.h"
#include "packet_handler.h"


class ThreadPool;
class TcpSession;


class WorkerThread
{
public:
	WorkerThread() {};
	WorkerThread(PACKET_FUNC callback, uint16_t thread_count, uint16_t interval);
	~WorkerThread();

public:
	void AddQueue(Packet packet);

private:
	void Worker(uint16_t thread_index);
	virtual void WorkerAfter() {};

private:
	uint16_t interval_;
	ThreadPool* thread_pool;
	ThreadSafeQueue<Packet> event_queue_;
	std::vector<Packet> event_list_;
	PACKET_FUNC callback_;
};
