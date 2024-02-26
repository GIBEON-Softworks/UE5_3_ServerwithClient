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
		HandleLoginResponse(true);
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
void ACGoogleLoginActor::HandleLoginResponse(bool bSuccess)
{

	// �α����� �������� ���� ó��
	if (bSuccess)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("ChangeLevel"));
		// �α��� ���� �ÿ��� �α��� ���¸� �����մϴ�.
		bIsLoggedIn = true;
		///Script/UMGEditor.WidgetBlueprint'/Game/HitUP/Widgets/Loading/WB_Loading.WB_Loading'
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


//���� ����
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




/* ���̺� üũ ���� �ְ� �α��� �۾��� �������� ������ �α����� �Ϸ�� ����� �������� �ٸ� ���̺� �����Ѵ�.
�ٸ� ���̺� ���� ���� ���� ���� �ִٸ� �α����� �ǰ� ���ٸ� �α����� ���� ���� ����
�α����� �Ϸ�Ǹ� 2000�� �ڵ带 ���� ��ȯ���� ���� ��ȯ���� JWT ��ū�� �� �ִ�.
*/