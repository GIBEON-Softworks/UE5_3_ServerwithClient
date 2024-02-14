// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NetActor.generated.h"

class Connector;
class ACSampleCharacter;
class ANetOtherCharacter;
struct BasePacket;

UCLASS()
class HITUP_API ANetActor : public AActor
{
	GENERATED_BODY()

public:
	ANetActor();
	~ANetActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ReqLogin(FString ip, FString port, FString name);

	UFUNCTION(BlueprintCallable)
	void JoinRoom(int RoomNumber);

	UFUNCTION(BlueprintCallable)
	void ClearRoom();

private:
	void SendMove();
	void AddPlayer(uint64 AddSessionId, FString nickname);

private: // 네트워크 함수들
	void Login(BasePacket* packet);
	void ResJoinRoom(BasePacket* packet);
	void PlayerJoinRoom(BasePacket* packet);
	void OutRoom(BasePacket* packet);
	void Move(BasePacket* packet);
	void SendAnimation(BasePacket* packet);

private:
	Connector* Client;
	uint64 MySessionId;

	TMap<uint64, ANetOtherCharacter*> PlayerList;
	ACSampleCharacter* MyPlayer;
	FTimerHandle TimerHandle;

	TSubclassOf<APawn> PlayerClass;
	TSubclassOf<UUserWidget> LoginWidgetClass;
	TSubclassOf<UUserWidget> LobboyWidgetClass;
};
