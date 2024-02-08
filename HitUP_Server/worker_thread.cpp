#include "worker_thread.h"
#include "thread_pool.h"


WorkerThread::WorkerThread(PACKET_FUNC callback, uint16_t thread_count, uint16_t interval)
	:callback_(callback),
	interval_(interval),
	thread_pool(nullptr)
{
	thread_pool = new ThreadPool(THREAD_FUNC_C(WorkerThread::Worker), thread_count);
}

WorkerThread::~WorkerThread()
{
	delete thread_pool;
}

void WorkerThread::AddQueue(Packet packet)
{
	event_queue_.Push(packet);
}


void WorkerThread::Worker(uint16_t thread_index)
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(interval_));

		event_queue_.DrainQueue(event_list_);

		for (auto packet : event_list_)
		{
			callback_(packet);
		}

		event_list_.clear();

		WorkerAfter();
	}
}
