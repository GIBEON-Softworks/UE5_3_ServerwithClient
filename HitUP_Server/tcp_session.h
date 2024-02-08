#pragma once
#include <mutex>
#include <atomic>
#ifndef _WINSOCK2API_
#include <WinSock2.h>
#include <Windows.h>
#endif
#define DATA_BUFSIZE 2048


class  StreamBuffer;


struct IO_DATA;


enum class IO_TYPE
{
	RECV,
	SEND
};

struct IO_DATA
{
	WSAOVERLAPPED overlapped;
	char buffer[DATA_BUFSIZE];
	WSABUF dataBuf;
	IO_TYPE ioType;
};

class SpinLock {

public:
	SpinLock()
		:lockFlag(false)
	{
	}

private:
	std::atomic<bool> lockFlag;

public:
	void lock() {
		bool expected = false;
		while (!lockFlag.compare_exchange_strong(expected, true)) {
			expected = false;
		}
	}

	void unlock() {
		lockFlag.store(false);
	}
};

class TcpSession
{
public:
	TcpSession();
	~TcpSession();

public:
	void Init(unsigned __int64 socket);
	void Close();

	// recv ��û �Լ�
	const int CallRecv();

	// send ��û �Լ�
	const int CallSend(void* buffer, int size);

	// ���� recv�� �Ϸ�Ǿ�����
	size_t Recv(size_t recv_byte);

	// ���� send�� �Ϸ�Ǿ�����
	void Send();

public:
	inline const int GetLastError()
	{
		return last_error_code_;
	}

	inline const unsigned __int64 GetSocket()
	{
		return socket_;
	}

	inline StreamBuffer* GetStreamBuffer()
	{
		return recv_buffer_;
	}

private:
	unsigned __int64 socket_;

	IO_DATA* send_info_;
	IO_DATA* recv_info_;
	StreamBuffer* recv_buffer_;
	SpinLock* send_lock_;

	unsigned long flags_;
	int last_error_code_;
	bool close_;

	std::atomic<uint16_t> call_count_;
};
