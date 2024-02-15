// Fill out your copyright notice in the Description page of Project Settings.


#include "Connector.h"
#include "Networking.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "StreamBuffer.h"

Connector::Connector()
	:Socket(nullptr), Buffer(nullptr), Thread(nullptr)
{
}

Connector::~Connector()
{
	if (Socket != nullptr)
	{
		Socket->Close();
		delete Socket;
	}

	if (Buffer != nullptr)
		delete Buffer;


	Buffer = nullptr;
	Socket = nullptr;

	if (Thread)
	{
		Thread->Kill();
	}

	for (auto It = PacketPool.CreateConstIterator(); It; ++It)
	{
		auto queue = It.Value();
		TArray<BasePacket*> packet_list;
		queue->DrainQueue(packet_list);

		for (auto packet : packet_list)
			delete packet;

		delete queue;
	}

	PacketPool.Reset();
}


bool Connector::Connection(FString host, int32 port, uint16 buffer_size)
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	// IP를 FString으로 입력받아 저장
	FString address = host;
	FIPv4Address ip;
	FIPv4Address::Parse(address, ip);

	// 포트와 소켓을 담는 클래스
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	addr->SetPort(port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Trying to connect.")));

	// 연결시도, 결과를 받아옴
	bool isConnetcted = Socket->Connect(*addr);

	Buffer = new StreamBuffer(buffer_size);
	Thread = FRunnableThread::Create(this, TEXT("sock"));
	return isConnetcted;
}

bool Connector::Init()
{
	return true;
}

// 스레드
uint32 Connector::Run()
{
	if (Socket != nullptr)
	{
		while (Socket != nullptr && Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
		{
			int32 BufferSize = Buffer->GetExtraSize();
			int32 BytesRead = 0;
			if (Socket->Recv(Buffer->TailBuffer(), BufferSize, BytesRead))
			{
				FString st = TEXT("Recv ") + FString::FromInt(BytesRead);
				UE_LOG(LogTemp, Log, TEXT("%s"), *st);
				Buffer->Enqueue(BytesRead);

				GeneratedPacket();
			}

			if (BytesRead == 0) break;

			FPlatformProcess::Sleep(0.03F);
		}
	}

	return 0;
}

void Connector::Send(BasePacket* packet)
{
	int32 SendLen = 0;
	Socket->Send((uint8*)packet, packet->packet_len, SendLen);
}

void Connector::Stop()
{
	if(Socket != nullptr)
		Socket->Close();
}

bool Connector::GeneratedPacket()
{
	while (true)
	{
		uint16 BufferSize = Buffer->GetSize();
		if (BufferSize < 4) break;

		BasePacket* header = (BasePacket*)(Buffer->PeekBuffer());

		if (BufferSize < header->packet_len) break;

		auto pool = PacketPool.Find(header->packet_id);

		if (pool == nullptr)
			return false;

		BasePacket* packet;
		(*pool)->TryPop(packet);

		Buffer->Dequeue((uint8*)packet, packet->packet_len);
		//Queue에 Insert
		Packet warpper;
		warpper.pool = *pool;
		warpper.packet = packet;

		MessageQueue.Push(warpper);
	}
	

	return true;
}