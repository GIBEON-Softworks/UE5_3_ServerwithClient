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
	// �α��� ���� Ȯ�� ���� ����
	bool bLoginCompleted = false; // �α����� �Ϸ�Ǿ����� ���θ� �������� �����´ٰ� ����

	if (!bIsLoggedIn)
	{
		FString TableCheckUrl = "http://hitup.shop:8000/login/tablecheck/" + RandomValue;

		// Send an HTTP request to check the table with the random number
		SendHttpRequest(TableCheckUrl, TEXT("GET"), TEXT(""));

	}

	if (bLoginCompleted)
	{
		// �α����� �Ϸ�Ǹ� Ÿ�̸� ����
		StopLoginCheckTimer();

		// �α��� �Ϸ� ó��
		HandleLoginResponse(true, ResponseCode);
	}
}


void ACGoogleLoginActor::OpenChromeBrowser(const FString& URL)
{
	if (!bIsLoggedIn)
	{
		FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);
		// �α��� üũ Ÿ�̸� ����
		StartLoginCheckTimer();
	}
}

// ���� �α��� �������� �α��� ����� ó���Ͽ� ���� ���� ���� �۾��� �����ϴ� ������ �մϴ�.
void ACGoogleLoginActor::HandleLoginResponse(bool bSuccess, int32 Code)
{
	if (!bSuccess)
	{
		switch (Code)
		{
		case 2000:
			// �α��� ����
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Login Success"));
			// ���� ȭ������ ��ȯ
			// ...
			break;
		case 4000:
			// �߸��� ��û
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Login Failed: Bad Request"));
			break;
		case 4400:
			// �Ҽ� �α��� ���� ����
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Login Failed: Social Login Info Not Found"));
			break;
		case 4401:
			// �߸��� ����Ű ����
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Login Failed: Incorrect Auth Key"));
			break;
		case 4402:
			// �������� �ʴ� ����Ű ����
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Login Failed: Non-Existent Auth Key"));
			break;
		case 4403:
			// ����Ű ��ȿ�Ⱓ ����
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Login Failed: Auth Key Expired"));
			break;
		default:
			// �� �� ���� ����
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Login Failed: Unknown Error"));
			break;
		}
	}
	else
	{
		// �α��� ����
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Login Failed this is HandleLoginResponse"));
	}



	// �α����� �������� ���� ó��
	//if (bSuccess)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ChangeLevel"));
	//	// �α��� ���� �ÿ��� �α��� ���¸� �����մϴ�.
	//	bIsLoggedIn = true;
	//	///Script/UMGEditor.WidgetBlueprint'/Game/HitUP/Widgets/Loading/WB_Loading.WB_Loading'
	//	// ���� ������ �̵��ϰų� Ư�� ������ ǥ���ϴ� ���� �۾��� ����
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
	// HTTP ��û ����
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindUObject(this, &ACGoogleLoginActor::HandleGoogleLoginResponse);
	Request->ProcessRequest();
	Request->GetResponse();
	
}


// HTTP ��û�� ������ ó���Ͽ� �������� ��� ����� Ȯ���ϴ� ����
void ACGoogleLoginActor::HandleGoogleLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	//int32 Responsecodes = Response->GetResponseCode();
	ResponseCode = Response->GetResponseCode();
	ResponseBody = Response->GetContentAsString();

	if (bWasSuccessful && Response.IsValid() && ResponseCode == EHttpResponseCodes::Ok)
	{
		/*FString ResponseDatas = Response->GetContentAsString();
		UE_LOG(LogTemp, Warning, TEXT("HTTP Response: %s"), *ResponseDatas);*/
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Login Success"));
		//UE_LOG(LogTemp, Warning, TEXT("ResponseCode: %s"), ResponseBody);

		
		// If login is successful, add logic to open a specific widget or move to the next level.
		HandleLoginResponse(true, ResponseCode);
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
		// ���� �α��� ��ư�� Ŭ���Ǿ��� ���� �����Դϴ�.
		FString GoogleLoginUrl = "http://hitup.shop:8000/google/login/";

		// ���⼭ ���� ���� �����ϰ� URL�� �߰����ݴϴ�.
		RandomValue = GenerateRandomString(10); // ������ ����
		GoogleLoginUrl += RandomValue;

		// HTTP ��û�� �����ϴ�.
		SendHttpRequest(GoogleLoginUrl, TEXT("GET"), TEXT(""));

		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, GoogleLoginUrl);
		// Chrome ������ ����
		OpenChromeBrowser(GoogleLoginUrl);
	}
}



/// <summary>
/// Timer Check
/// </summary>
void ACGoogleLoginActor::StartLoginCheckTimer()
{
	// �α��� ���� Ȯ���� ���� Ÿ�̸� ����
	GetWorldTimerManager().SetTimer(LoginCheckTimerHandle, this, &ACGoogleLoginActor::CheckLoginStatus, 1.0f, true);
	
}

void ACGoogleLoginActor::StopLoginCheckTimer()
{
	// Ÿ�̸� ����
	GetWorldTimerManager().ClearTimer(LoginCheckTimerHandle);
}




////////////////////////////////////////////////////////////////////////


//���� ���� ��
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
