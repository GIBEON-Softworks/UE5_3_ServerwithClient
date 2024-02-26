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
	// Google 로그인 버튼 클릭 시 호출되는 함수
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

	// HTTP 요청 함수
	void SendHttpRequest(const FString& Url, const FString& Verb, const FString& Content);

	// Google 로그인 요청 처리 함수
	void HandleGoogleLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// 크롬 브라우저 열기
	void OpenChromeBrowser(const FString& URL);

	void HandleLoginResponse(bool bSuccess, int32 Code);

	// 난수 생성 함수
	FString GenerateRandomString(int32 Length);

	FTimerHandle LoginCheckTimerHandle;

private:
	bool bIsLoggedIn = false; // 사용자의 로그인 상태를 나타내는 변수


public:
	FString RandomValue;

	// 로그인 성공 응답 코드 추출 (예: 200)
	int32 ResponseCode;
	FString ResponseBody;
};
