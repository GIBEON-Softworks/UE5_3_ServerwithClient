#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Http.h"
#include "CGoogleLoginActor.generated.h"

UCLASS()
class HITUP_API ACGoogleLoginActor : public AActor
{
	GENERATED_BODY()

public:
    ACGoogleLoginActor();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override; // Tick 함수 선언

public:
    UFUNCTION(BlueprintCallable)
    void OnGoogleLoginButtonClicked();

    // 레벨 이동 함수
    UFUNCTION(BlueprintCallable)
    void MoveToNewLevel(const FString& LevelName);

private:
    void OpenChromeBrowser(const FString& URL);
    void OnBrowserClosed(const FString& URL);
    void HandleLoginResponse(bool bSuccess, int32 Code);
    FString GenerateRandomString(int32 Length);
    void SendHttpRequest(const FString& URL, const FString& Verb, const FString& Content);
    void HandleGoogleLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
    void CheckTableCode(const FString& Url, const FString& Content);
    void HandleTableCodeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
    void CheckTableCodePeriodically(); // 주기적으로 Tablecheck 확인하는 함수
    void HandleCheckTableCodeTimer(); // 임시용 테이블

private:
    FString RandomValue;
    bool bIsLoggedIn;
    int32 ResponseCode;

    FHttpModule* HttpModule;
    FTimerHandle LoginCheckTimerHandle; // 타이머 핸들

    // Tablecheck 주기적 확인을 위한 변수
    bool bIsCheckingTableCode;
    FString LastTableCheckUrl;

private:
    FString GoogleLoginUrl;

   

};
