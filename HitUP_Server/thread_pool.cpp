#include "thread_pool.h"

ThreadPool::ThreadPool(THREAD_FUNC func, uint16_t thread_count)
	:worker_func_(func), thread_count_(thread_count)
{
	Init();
}

ThreadPool::~ThreadPool()
{
	Release();
}

void ThreadPool::Join()
{
	for (auto info : thread_list_)
	{
		info->join();
	}
}

void ThreadPool::Init()
{
	for (uint16_t i = 0; i < thread_count_; i++)
	{
		thread_list_.push_back(new std::thread(worker_func_, i));
	}
}

void ThreadPool::Release()
{
	if (thread_list_.size() == 0)
		return;

	for (auto info : thread_list_)
	{
		delete info;
	}

}