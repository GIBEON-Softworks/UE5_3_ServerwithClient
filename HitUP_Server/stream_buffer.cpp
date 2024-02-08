#include "stream_buffer.h"
#include <windows.h>

StreamBuffer::StreamBuffer()
	:max_(STREAM_BUFSIZE)
{
	ZeroMemory(buffer_, STREAM_BUFSIZE);
	head_ = 0;
	tail_ = 0;
	size_ = 0;
}

StreamBuffer::~StreamBuffer()
{

}

size_t StreamBuffer::Enqueue(char* data, size_t data_size)
{
	if (max_ - size_ < data_size)
		return 0;

	// ���� ������ ������
	size_t available_size = 0;
	available_size = max_ - tail_;;

	if (data_size > available_size)
	{
		Shift();
	}


	// ���� ������ ������ ��ŭ ����
	memcpy_s(&buffer_[tail_], available_size, data, data_size);

	tail_ += data_size;
	size_ += data_size;

	return data_size;
}

void StreamBuffer::Dequeue(char* get_buffer, size_t get_size)
{
	// ���� ������ ������
	size_t available_size = 0;
	available_size = tail_ - head_;

	if (available_size < get_size)
		available_size = get_size;

	memcpy_s(get_buffer, get_size, &buffer_[head_], get_size);


	size_ -= get_size;
	head_ += get_size;

	Shift();
}

void StreamBuffer::Shift()
{
	if (size_ == 0)
	{
		size_ = 0;
		tail_ = 0;
		head_ = 0;
		return;
	}

	memcpy_s(buffer_, max_, &buffer_[head_], size_);
	head_ = 0;
	tail_ = size_;
}