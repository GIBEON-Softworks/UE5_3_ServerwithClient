// Fill out your copyright notice in the Description page of Project Settings.


#include "StreamBuffer.h"


StreamBuffer::StreamBuffer(uint16 buffer_size)
	:max_(buffer_size), buffer_(nullptr)
{
	buffer_ = new uint8[buffer_size];
	head_ = 0;
	tail_ = 0;
	size_ = 0;
}

StreamBuffer::~StreamBuffer()
{
	if(buffer_ != nullptr)
		delete[] buffer_;
}

size_t StreamBuffer::Enqueue(size_t data_size)
{
	tail_ += data_size;
	size_ += data_size;
	if (tail_ == max_)
	{
		Shift();
	}

	return data_size;
}

void StreamBuffer::Dequeue(uint8* get_buffer, size_t get_size)
{
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