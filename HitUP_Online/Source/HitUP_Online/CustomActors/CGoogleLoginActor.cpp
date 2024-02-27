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

	bIsLoggedIn = false; // �α��� ���� �ʱ�ȭ
}

/**
*
*/

void ACGoogleLoginActor::CheckLoginStatus()
{
	// �α��� ���� Ȯ�� ���� ����
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
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Login Failed"));
	}

	// �α����� �������� ���� ó��
	if (bSuccess)
	{
		// �α��� ���� �ÿ��� �α��� ���¸� �����մϴ�.
		bIsLoggedIn = true;

		// ���� ������ �̵��ϰų� Ư�� ������ ǥ���ϴ� ���� �۾��� ����
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

	// Ÿ�̸� ����
	GetWorldTimerManager().ClearTimer(LoginCheckTimerHandle);



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
	if (bWasSuccessful && Response.IsValid() && Response->GetResponseCode() == EHttpResponseCodes::Ok)
	{
		FString ResponseData = Response->GetContentAsString();


		// JSON �Ľ�
		//TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create("");
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(ResponseData);

		if (JsonObject.IsValid())
		{
			// �α��� ���� ���� Ȯ��
			bool bIsLoggedIn = true;
			int32 ResponseCode = JsonObject->GetIntegerField("code");

			HandleLoginResponse(bIsLoggedIn, ResponseCode);
		}
		else
		{
			// JSON �Ľ� ���� ó��
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("JSON Parsing Error"));
		}
	}
	else
	{
		// �α��� ����
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Login Failed"));
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
