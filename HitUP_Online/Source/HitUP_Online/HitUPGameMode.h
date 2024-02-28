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

	//UFUNCTION(BlueprintCallable, Category = "UMG_GAME")
	//static void ChangeLevel(const FString& LevelName);

	UFUNCTION(BlueprintCallable, Category = "UMG_GAME")
	void CalledWeb();

	UFUNCTION(BlueprintCallable, Category = "UMG_GAME")
	void CallLogin(const FString& Id, const FString& Password);

	UFUNCTION(BlueprintCallable, Category = "UMG_GAME")
	void CallJoin(const FString& Id, const FString& Address, const FString& Password);

	UFUNCTION(BlueprintCallable, Category = "UMG_GAME")
	void ClickJoin(const FString& token, const int32 click_Point);

	UFUNCTION(BlueprintCallable, Category = "UMG_GAME")
	void CallRank();


private:
	// Login 요청
	void LoginOnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
	// Join 요청
	void JoinOnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
	// click 요청
	void ClickOnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);
	// Rank 요청
	void RankOnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

	static void ChangeLevel(const FString& LevelName, UWorld* World);



protected:
	virtual void BeginPlay() override;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	//TSubclassOf<UUserWidget> StartingWidgetClass;

	UPROPERTY()
	UUserWidget* CurrentWidget;

	UPROPERTY()
	FString my_token; // token을 저장할 공간

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FString TransferLevelName;

};



