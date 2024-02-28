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
	// �α��ν� ����� ��ū ������
	FString token;

	// �����͸� �����ϴ� �Լ�
	void saveToken(const FString& newToken);

	// �����͸� �ҷ����� �Լ�
	FString LoadData();
	
	// ��ŷ �����͸� ��� �迭
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RankList")
	TArray<FString> vRank_data;
};
