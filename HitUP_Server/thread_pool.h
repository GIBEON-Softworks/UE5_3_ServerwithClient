#pragma once
#include <vector>
#include <functional>
#include <thread>

#define THREAD_FUNC_C(f) std::bind(&f, this, std::placeholders::_1)
typedef std::function<void(uint16_t)> THREAD_FUNC;

class ThreadPool
{
public:
	ThreadPool(THREAD_FUNC func, uint16_t thread_count);
	~ThreadPool();

public:
	void Join();

	inline uint16_t GetThreadCount()
	{
		return thread_count_;
	}

private:
	void Init();
	void Release();

private:
	uint16_t thread_count_;

	THREAD_FUNC worker_func_;
	std::vector<std::thread*> thread_list_;
};
