// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SAMPLE_API StreamBuffer
{
public:
	StreamBuffer(uint16 buffer_size);
	~StreamBuffer();

public:
	size_t Enqueue(size_t size);
	void Dequeue(uint8* get_buffer, size_t get_size);
	void Shift();

	inline size_t GetSize()
	{
		return size_;
	}

	inline size_t GetExtraSize()
	{
		return max_ - size_;
	}

	inline uint8* PeekBuffer()
	{
		return &buffer_[head_];
	}

	inline uint8* TailBuffer()
	{
		return &buffer_[tail_];
	}

private:
	uint8* buffer_;
	size_t head_;
	size_t tail_;
	size_t size_;
	const size_t max_;
};
