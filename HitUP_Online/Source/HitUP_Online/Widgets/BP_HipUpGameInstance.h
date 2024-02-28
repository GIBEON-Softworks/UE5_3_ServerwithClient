// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BP_HipUpGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class HITUP_API UBP_HipUpGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
	// 로그인시 저장될 토큰 데이터
	FString token;

	// 데이터를 저장하는 함수
	void saveToken(const FString& newToken);

	// 데이터를 불러오는 함수
	FString LoadData();
	
	// 랭킹 데이터를 담는 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RankList")
	TArray<FString> vRank_data;
};
