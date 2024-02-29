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
    virtual void Tick(float DeltaTime) override; // Tick �Լ� ����

public:
    UFUNCTION(BlueprintCallable)
    void OnGoogleLoginButtonClicked();

    // ���� �̵� �Լ�
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
    void CheckTableCodePeriodically(); // �ֱ������� Tablecheck Ȯ���ϴ� �Լ�
    void HandleCheckTableCodeTimer(); // �ӽÿ� ���̺�

private:
    FString RandomValue;
    bool bIsLoggedIn;
    int32 ResponseCode;

    FHttpModule* HttpModule;
    FTimerHandle LoginCheckTimerHandle; // Ÿ�̸� �ڵ�

    // Tablecheck �ֱ��� Ȯ���� ���� ����
    bool bIsCheckingTableCode;
    FString LastTableCheckUrl;

private:
    FString GoogleLoginUrl;

   

};
