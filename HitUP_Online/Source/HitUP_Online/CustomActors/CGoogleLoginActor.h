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
	// Sets default values for this actor's properties
	ACGoogleLoginActor();

public:
	// Google �α��� ��ư Ŭ�� �� ȣ��Ǵ� �Լ�
	UFUNCTION(BlueprintCallable)
	void OnGoogleLoginButtonClicked();

	UFUNCTION(BlueprintCallable)
	void StartLoginCheckTimer();

	UFUNCTION(BlueprintCallable)
	void StopLoginCheckTimer();

protected:
	virtual void BeginPlay() override;

private:
	void CheckLoginStatus();

	// HTTP ��û �Լ�
	void SendHttpRequest(const FString& Url, const FString& Verb, const FString& Content);

	// Google �α��� ��û ó�� �Լ�
	void HandleGoogleLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// ũ�� ������ ����
	void OpenChromeBrowser(const FString& URL);

	void HandleLoginResponse(bool bSuccess, int32 Code);

	// ���� ���� �Լ�
	FString GenerateRandomString(int32 Length);

	FTimerHandle LoginCheckTimerHandle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLoggedIn; // ������� �α��� ���¸� ��Ÿ���� ����

	// �α��� ���� ���� �ڵ� ���� (��: 200)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ResponseCode;

public:
	FString RandomValue;


	FString ResponseBody;
};
