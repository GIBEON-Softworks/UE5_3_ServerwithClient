#include "CustomActors/CGoogleLoginActor.h"
#include "Http.h"
#include "Blueprint/UserWidget.h"

ACGoogleLoginActor::ACGoogleLoginActor()
{
}


void ACGoogleLoginActor::BeginPlay()
{
	Super::BeginPlay();

}

void ACGoogleLoginActor::CheckLoginStatus()
{
	// 로그인 상태 확인 로직 구현
	bool bLoginCompleted = false; // 로그인이 완료되었는지 여부를 서버에서 가져온다고 가정

	if (!bIsLoggedIn)
	{
		FString TableCheckUrl = "http://hitup.shop:8000/login/tablecheck/" + RandomValue;

		// Send an HTTP request to check the table with the random number
		SendHttpRequest(TableCheckUrl, TEXT("GET"), TEXT(""));

	}

	if (bLoginCompleted)
	{
		// 로그인이 완료되면 타이머 중지
		StopLoginCheckTimer();

		// 로그인 완료 처리
		HandleLoginResponse(true);
	}


}


void ACGoogleLoginActor::OpenChromeBrowser(const FString& URL)
{
	if (!bIsLoggedIn)
	{
		FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);
		// 로그인 체크 타이머 시작
		StartLoginCheckTimer();
	}
}

// 실제 로그인 로직에서 로그인 결과를 처리하여 게임 내의 다음 작업을 결정하는 역할을 합니다.
void ACGoogleLoginActor::HandleLoginResponse(bool bSuccess)
{

	// 로그인이 성공했을 때의 처리
	if (bSuccess)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ChangeLevel"));
		// 로그인 성공 시에만 로그인 상태를 변경합니다.
		bIsLoggedIn = true;
		///Script/UMGEditor.WidgetBlueprint'/Game/HitUP/Widgets/Loading/WB_Loading.WB_Loading'
		// 다음 레벨로 이동하거나 특정 위젯을 표시하는 등의 작업을 수행
		TSubclassOf<UUserWidget> GameStartWidgetClass = LoadClass<UObject>(nullptr, TEXT("/Game/HitUP/Widgets/Loading/WB_Loading.WB_Loading"));
		if (GameStartWidgetClass)
		{
			UUserWidget* GameStartWidget = CreateWidget<UUserWidget>(GetWorld(), GameStartWidgetClass);
			// Add the widget to viewport
			GameStartWidget->AddToViewport();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Failed to load Game Start Widget"));
		}
	}
}

void ACGoogleLoginActor::SendHttpRequest(const FString& Url, const FString& Verb, const FString& Content)
{
	// HTTP 요청 생성
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindUObject(this, &ACGoogleLoginActor::HandleGoogleLoginResponse);
	Request->ProcessRequest();
	Request->GetResponse();
	
}


// HTTP 요청과 응답을 처리하여 서버와의 통신 결과를 확인하는 역할
void ACGoogleLoginActor::HandleGoogleLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid() && Response->GetResponseCode() == EHttpResponseCodes::Ok)
	{
		/*FString ResponseDatas = Response->GetContentAsString();
		UE_LOG(LogTemp, Warning, TEXT("HTTP Response: %s"), *ResponseDatas);*/
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Login Success"));

		// If login is successful, add logic to open a specific widget or move to the next level.
		HandleLoginResponse(true);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Login Failed"));
		// When login fails, the failure message is displayed as a floating widget.
		//TSubclassOf<UUserWidget> LoginFailedWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT(""));
		//if (LoginFailedWidgetClass)
		//{
			//UUserWidget* LoginFailedWidget = CreateWidget<UUserWidget>(GetWorld(), LoginFailedWidgetClass);
			// Add a widget to a specific location.
			//LoginFailedWidget->AddToViewport();
		//}
		//else
		//{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Failed to load Login Failed Widget"));
		//}
	}
}

void ACGoogleLoginActor::OnGoogleLoginButtonClicked()
{
	if (!bIsLoggedIn)
	{
		// 구글 로그인 버튼이 클릭되었을 때의 동작입니다.
		FString GoogleLoginUrl = "http://hitup.shop:8000/google/login/";

		// 여기서 난수 값을 생성하고 URL에 추가해줍니다.
		RandomValue = GenerateRandomString(10); // 랜덤값 생성
		GoogleLoginUrl += RandomValue;

		// HTTP 요청을 보냅니다.
		SendHttpRequest(GoogleLoginUrl, TEXT("GET"), TEXT(""));

		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, GoogleLoginUrl);
		// Chrome 브라우저 열기
		OpenChromeBrowser(GoogleLoginUrl);
	}
}

void ACGoogleLoginActor::StartLoginCheckTimer()
{
	// 로그인 상태 확인을 위한 타이머 시작
	GetWorldTimerManager().SetTimer(LoginCheckTimerHandle, this, &ACGoogleLoginActor::CheckLoginStatus, 1.0f, true);
	
}

void ACGoogleLoginActor::StopLoginCheckTimer()
{
	// 타이머 중지
	GetWorldTimerManager().ClearTimer(LoginCheckTimerHandle);
}


//난수 생성
FString ACGoogleLoginActor::GenerateRandomString(int32 Length)
{
	const FString AllowedCharacters = TEXT("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

	FString RandomString;

	for (int32 i = 0; i < Length; ++i)
	{
		RandomString.AppendChar(AllowedCharacters[FMath::RandRange(0, AllowedCharacters.Len() - 1)]);
	}

	return RandomString;
}




/* 테이블 체크 난수 주고 로그인 작업이 서버에서 끝나면 로그인이 완료된 사람의 난수값을 다른 테이블에 저장한다.
다른 테이블에 지금 가진 난수 값이 있다면 로그인이 되고 없다면 로그인이 아직 되지 않음
로그인이 완료되면 2000이 코드를 가진 반환값이 오고 반환값에 JWT 토큰이 들어가 있다.
*/