// Fill out your copyright notice in the Description page of Project Settings.


#include "NetActor.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextRenderComponent.h"

#include "HitUP_Online/ClientSystem/Connector.h"
#include "HitUP_Online/ClientSystem/game_packet.h"
#include "HitUP_Online/Characters/CSampleCharacter.h"
#include "HitUP_Online/Characters/NetOtherCharacter.h"

// Sets default values
ANetActor::ANetActor()
	:Client(nullptr), MyPlayer(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	///Script/Engine.Blueprint'/Game/HitUP/Widgets/Actors/BPC_NetActor.BPC_NetActor'
	static ConstructorHelpers::FClassFinder<APawn> NetBp(TEXT("/Game/HitUP/Widgets/Actors/BPC_NetActor"));
	if (NetBp.Class != NULL)
	{
		PlayerClass = NetBp.Class;
	}
}

ANetActor::~ANetActor()
{
	if(Client!=nullptr)
		delete Client;
}

// Called when the game starts or when spawned
void ANetActor::BeginPlay()
{
	Super::BeginPlay();

	// 클라이언트 생성및 패킷 등록
	Client = new Connector;
	Client->AddPacket<ResLoginPacket>(20);
	Client->AddPacket<ResJoinRoomPacket>(20);
	Client->AddPacket<BroadJoinRoomPacket>(20);
	Client->AddPacket<BroadOutRoomPacket>(20);
	Client->AddPacket<ResMovePacket>(2000);
}

// Called every frame
void ANetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TArray<Packet> MessageList;
	Client->GetMessage(MessageList);

	for (auto item : MessageList)
	{
		switch (item.packet->packet_id)
		{
		case GAME_PACKET::RES_LOGIN:
			Login(item.packet);
			break;

		case GAME_PACKET::RES_JOIN_ROOM:
			ResJoinRoom(item.packet);
			break;

		case GAME_PACKET::BROAD_JOIN_ROOM:
			PlayerJoinRoom(item.packet);
			break;

		case GAME_PACKET::BROAD_OUT_ROOM:
			OutRoom(item.packet);
			break;

		case GAME_PACKET::RES_MOVE:
			Move(item.packet);
			break;
		}

		item.Relese();
	}
}

//for Widget Login
void ANetActor::ReqLogin(FString ip, FString port, FString name)
{
	// 커낵션 확인
	UE_LOG(LogTemp, Log, TEXT("Start Connectioin"));
	int portnumber = FCString::Atoi(*port);
	bool bconnected = Client->Connection(ip, portnumber);
	UE_LOG(LogTemp, Log, TEXT("End Connectioin %s"), bconnected ? TEXT("true") : TEXT("false"));

	ReqLoginPacket login;
	strcpy_s(login.name, TCHAR_TO_ANSI(*name));
	Client->Send(&login);
}

void ANetActor::JoinRoom(int RoomNumber)
{
	ReqJoinRoomPacket room;
	room.room_id = RoomNumber;
	Client->Send(&room);

	//월드상 움직임을 주는 값
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ANetActor::SendMove, 0.05f, true);
}

void ANetActor::ClearRoom()
{
	// 타이머 제거
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	for (auto player : PlayerList)
	{
		player.Value->Destroy();
	}

	PlayerList.Empty();
}

void ANetActor::SendMove()
{
	if (MyPlayer == nullptr) return;
	ReqMovePacket move;

	FVector3d v = MyPlayer->GetActorLocation();
	move.x = v.X;
	move.y = v.Y;
	move.z = v.Z;

	Client->Send(&move);
}

void ANetActor::AddPlayer(uint64 AddSessionId, FString nickname)
{
	if (MySessionId == AddSessionId) return;

	auto actor = PlayerList.Find(AddSessionId);

	if (actor != nullptr) return;

	auto player = Cast<ANetOtherCharacter>(GetWorld()->SpawnActor<APawn>(PlayerClass, FVector(1000, 1000, 88), FRotator(0, 0, 0)));

	TArray<UTextRenderComponent*> Components;
	player->GetComponents<UTextRenderComponent>(Components);

	for (UTextRenderComponent* TextComponent : Components)
	{
		if (TextComponent)
		{
			TextComponent->SetText(FText::FromString(nickname));
		}
	}

	PlayerList.Add(AddSessionId, player);
}

void ANetActor::Login(BasePacket* packet)
{
	auto loginpacket = reinterpret_cast<ResLoginPacket*>(packet);
	FString string = TEXT("로그인 ") + FString::FromInt(loginpacket->session_id);

	MySessionId = loginpacket->session_id;
	UE_LOG(LogTemp, Log, TEXT("%s"), *string);
}

void ANetActor::ResJoinRoom(BasePacket* packet)
{
	auto roompacket = reinterpret_cast<ResJoinRoomPacket*>(packet);
	FString string = TEXT("방입장 ") + FString(roompacket->success ? TEXT("성공") : TEXT("실패"));

	UE_LOG(LogTemp, Log, TEXT("%s"), *string);

	if (roompacket->success == false) return;

	// 현재 플레이어 캐릭터를 가져오기
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	// AMyCharacter로 캐스팅하기
	MyPlayer = Cast<ACSampleCharacter>(PlayerCharacter);

	for (int i = 0; i < 10; i++)
	{
		int64_t session_id = roompacket->session_list[i];
		char* nickname = roompacket->name[i];
		if (session_id == 0) break;

		AddPlayer(session_id, nickname);
	}

}

void ANetActor::PlayerJoinRoom(BasePacket* packet)
{
	auto roompacket = reinterpret_cast<BroadJoinRoomPacket*>(packet);
	if (roompacket->session_id == MySessionId)
		return;

	FString string = TEXT("플레이어입장 " + FString::FromInt(roompacket->session_id));
	UE_LOG(LogTemp, Log, TEXT("%s"), *string);

	AddPlayer(roompacket->session_id, roompacket->name);
}

void ANetActor::OutRoom(BasePacket* packet)
{
	auto roompacket = reinterpret_cast<BroadOutRoomPacket*>(packet);
	auto sid = roompacket->session_id;

	auto actor = PlayerList.Find(roompacket->session_id);

	if (actor == nullptr) return;
	(*actor)->Destroy();
	PlayerList.Remove(roompacket->session_id);
}

void ANetActor::Move(BasePacket* packet)
{
	auto movepacket = reinterpret_cast<ResMovePacket*>(packet);
	auto actor = PlayerList.Find(movepacket->session_id);

	if (actor == nullptr) return;
	if (*actor == nullptr) return;

	FVector v;
	v.X = movepacket->x;
	v.Y = movepacket->y;
	v.Z = movepacket->z;
	(*actor)->SetTargetPos(v);
}