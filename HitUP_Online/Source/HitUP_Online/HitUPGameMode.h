// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "http.h"
#include "HitUPGameMode.generated.h"

UCLASS()
class HITUP_API AHitUPGameMode : public AGameModeBase
{
	GENERATED_BODY()


	AHitUPGameMode();

public:
	// 레벨별로 사용할 Start Widget 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My GameMode")
	TSubclassOf<UUserWidget> Level1StartWidgetClass; // login level start widget

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My GameMode")
	TSubclassOf<UUserWidget> Level2StartWidgetClass; // ingame level start widget


public:
	UFUNCTION(BlueprintCallable, Category = "UMG_GAME")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	UFUNCTION(BlueprintCallable, Category = "UMG_GAME")
	static void ChangeLevel(const FString& LevelName);

	UFUNCTION(BlueprintCallable, Category = "UMG_GAME")
	void CalledWeb();

	UFUNCTION(BlueprintCallable, Category = "UMG_GAME")
	void CallLogin(const FString& Id, const FString& Password);


private:
	void OnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
	FString HashString(const FString& InputString);


protected:
	virtual void BeginPlay() override;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	//TSubclassOf<UUserWidget> StartingWidgetClass;

	UPROPERTY()
	UUserWidget* CurrentWidget;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FString TransferLevelName;
};



