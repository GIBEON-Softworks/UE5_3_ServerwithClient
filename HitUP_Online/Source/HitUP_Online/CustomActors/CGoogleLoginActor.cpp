#include "CustomActors/CGoogleLoginActor.h"
#include "Http.h"
#include "Blueprint/UserWidget.h"
#include "Json.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"

ACGoogleLoginActor::ACGoogleLoginActor()
{
	//  Initialize bIsLoggedIn and ResponseCode here
	bIsLoggedIn = false;
	ResponseCode = 0; // Assuming ResponseCode is an integer
}


void ACGoogleLoginActor::BeginPlay()
{
	Super::BeginPlay();

	bIsLoggedIn = false; // 로그인 여부 초기화
}

/**
*
*/

void ACGoogleLoginActor::CheckLoginStatus()
{
	// 로그인 상태 확인 로직 구현
	FString TableCheckUrl = "http://hitup.shop:8000/login/tablecheck/" + RandomValue;

	// Send an HTTP request to check the table with the random number
	SendHttpRequest(TableCheckUrl, TEXT("GET"), TEXT(""));

	StartLoginCheckTimer();
}


void ACGoogleLoginActor::OpenChromeBrowser(const FString& URL)
{
	if (!bIsLoggedIn)
	{
		FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);
	}
}

// 실제 로그인 로직에서 로그인 결과를 처리하여 게임 내의 다음 작업을 결정하는 역할을 합니다.
void ACGoogleLoginActor::HandleLoginResponse(bool bSuccess, int32 Code)
{
	if (!bSuccess)
	{
		switch (Code)
		{
		case 2000:
			// 로그인 성공
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Login Success"));
			// 게임 화면으로 전환
			// ...
			break;
		case 4000:
			// 잘못된 요청
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Login Failed: Bad Request"));
			break;
		case 4400:
			// 소셜 로그인 정보 없음
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Login Failed: Social Login Info Not Found"));
			break;
		case 4401:
			// 잘못된 인증키 전송
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Login Failed: Incorrect Auth Key"));
			break;
		case 4402:
			// 존재하지 않는 인증키 전송
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Login Failed: Non-Existent Auth Key"));
			break;
		case 4403:
			// 인증키 유효기간 만료
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Login Failed: Auth Key Expired"));
			break;
		default:
			// 알 수 없는 오류
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Login Failed: Unknown Error"));
			break;
		}
	}
	else
	{
		// 로그인 실패
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Login Failed"));
	}

	// 로그인이 성공했을 때의 처리
	if (bSuccess)
	{
		// 로그인 성공 시에만 로그인 상태를 변경합니다.
		bIsLoggedIn = true;

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

	// 타이머 중지
	GetWorldTimerManager().ClearTimer(LoginCheckTimerHandle);



	// 로그인이 성공했을 때의 처리
	//if (bSuccess)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ChangeLevel"));
	//	// 로그인 성공 시에만 로그인 상태를 변경합니다.
	//	bIsLoggedIn = true;
	//	///Script/UMGEditor.WidgetBlueprint'/Game/HitUP/Widgets/Loading/WB_Loading.WB_Loading'
	//	// 다음 레벨로 이동하거나 특정 위젯을 표시하는 등의 작업을 수행
	//	TSubclassOf<UUserWidget> GameStartWidgetClass = LoadClass<UObject>(nullptr, TEXT("/Game/HitUP/Widgets/Loading/WB_Loading.WB_Loading"));
	//	if (GameStartWidgetClass)
	//	{
	//		UUserWidget* GameStartWidget = CreateWidget<UUserWidget>(GetWorld(), GameStartWidgetClass);
	//		// Add the widget to viewport
	//		GameStartWidget->AddToViewport();
	//	}
	//	else
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Failed to load Game Start Widget"));
	//	}
	//}
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
		FString ResponseData = Response->GetContentAsString();


		// JSON 파싱
		//TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create("");
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(ResponseData);

		if (JsonObject.IsValid())
		{
			// 로그인 성공 여부 확인
			bool bIsLoggedIn = true;
			int32 ResponseCode = JsonObject->GetIntegerField("code");

			HandleLoginResponse(bIsLoggedIn, ResponseCode);
		}
		else
		{
			// JSON 파싱 오류 처리
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("JSON Parsing Error"));
		}
	}
	else
	{
		// 로그인 실패
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Login Failed"));
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



/// <summary>
/// Timer Check
/// </summary>
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




////////////////////////////////////////////////////////////////////////


//난수 생성 완
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
