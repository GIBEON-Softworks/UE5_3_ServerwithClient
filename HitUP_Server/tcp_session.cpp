#include "tcp_session.h"
#include "stream_buffer.h"


TcpSession::TcpSession()
	:flags_(0),
	last_error_code_(0),
	socket_(0),
	close_(false),
	recv_buffer_(nullptr),
	send_info_(nullptr),
	recv_info_(nullptr),
	send_lock_(nullptr)

{
	send_info_ = new IO_DATA;
	recv_info_ = new IO_DATA;
	recv_buffer_ = new StreamBuffer;
	send_lock_ = new SpinLock;
}

TcpSession::~TcpSession()
{
	if (recv_buffer_ != nullptr)
		delete recv_buffer_;

	if (recv_info_ != nullptr)
		delete recv_info_;

	if (send_info_ != nullptr)
		delete send_info_;

	if (send_lock_ != nullptr)
		delete send_lock_;

	recv_buffer_ = nullptr;
	recv_info_ = nullptr;
	send_info_ = nullptr;
	send_lock_ = nullptr;
}


void TcpSession::Init(unsigned __int64 socket)
{
	flags_ = 0;
	socket_ = socket;

	// recv ���� �ʱ�ȭ
	recv_info_->dataBuf.buf = recv_info_->buffer;
	recv_info_->dataBuf.len = DATA_BUFSIZE;
	recv_info_->ioType = IO_TYPE::RECV;
	ZeroMemory(&(recv_info_->overlapped), sizeof(OVERLAPPED));

	// send ���� �ʱ�ȭ
	send_info_->dataBuf.buf = send_info_->buffer;
	send_info_->dataBuf.len = DATA_BUFSIZE;
	send_info_->ioType = IO_TYPE::SEND;
	ZeroMemory(&(send_info_->overlapped), sizeof(OVERLAPPED));

}

void TcpSession::Close()
{
	close_ = true;
	closesocket(socket_);
}

// recv ��û �Լ�
const int TcpSession::CallRecv()
{
	if (WSARecv(socket_, &recv_info_->dataBuf, 1, NULL, &flags_, &recv_info_->overlapped, NULL) == SOCKET_ERROR)
	{
		last_error_code_ = WSAGetLastError();
		if (last_error_code_ == WSA_IO_PENDING)
		{
			// ����
		}
		else
		{
			// ����	
		}

		return last_error_code_;
	}

	return false;
}

// send ��û �Լ�
const int TcpSession::CallSend(void* buffer, int size)
{
	// lock
	send_lock_->lock();

	// ���ۿ� ����
	memcpy_s(send_info_->buffer, DATA_BUFSIZE, buffer, size);
	send_info_->dataBuf.len = size;
	if (WSASend(socket_, &send_info_->dataBuf, 1, NULL, 0, &send_info_->overlapped, NULL) == SOCKET_ERROR)
	{
		last_error_code_ = WSAGetLastError();
		if (last_error_code_ == WSA_IO_PENDING)
		{
			// ����
		}
		else
		{
			// ����	
			send_lock_->unlock();
		}

		return last_error_code_;
	}

	return false;
}

// ���� recv�� �Ϸ�Ǿ�����
size_t TcpSession::Recv(size_t recv_byte)
{
	return recv_buffer_->Enqueue(recv_info_->buffer, recv_byte);
}

void TcpSession::Send()
{
	send_lock_->unlock();
}
