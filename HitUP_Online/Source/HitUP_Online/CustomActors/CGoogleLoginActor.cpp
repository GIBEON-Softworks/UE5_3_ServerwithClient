#include "CustomActors/CGoogleLoginActor.h"
#include "Http.h"
#include "Blueprint/UserWidget.h"

ACGoogleLoginActor::ACGoogleLoginActor()
{
}


void ACGoogleLoginActor::BeginPlay()
{
    Super::BeginPlay();

    // �α��� üũ Ÿ�̸� ����
    StartLoginCheckTimer();
}

void ACGoogleLoginActor::CheckLoginStatus()
{
    // �α��� ���� Ȯ�� ���� ����
    bool bLoginCompleted = false; // �α����� �Ϸ�Ǿ����� ���θ� �������� �����´ٰ� ����

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
    FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);
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

        // ���� ������ �̵��ϰų� Ư�� ������ ǥ���ϴ� ���� �۾��� ����
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
}


// HTTP ��û�� ������ ó���Ͽ� �������� ��� ����� Ȯ���ϴ� ����
void ACGoogleLoginActor::HandleGoogleLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid() && Response->GetResponseCode() == EHttpResponseCodes::Ok)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Login Success"));
        // �α��ο� ������ ���, Ư�� ������ ���ų� ���� ������ �̵��ϴ� ������ �߰��մϴ�.


        TSubclassOf<UUserWidget> LoginSuccessWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/HitUP/Widgets/Loading/WB_Loading"));
        if (LoginSuccessWidgetClass)
        {
            UUserWidget* LoginSuccessWidget = CreateWidget<UUserWidget>(GetWorld(), LoginSuccessWidgetClass);
            // Ư�� ��ġ�� ������ �߰��մϴ�.
            LoginSuccessWidget->AddToViewport();
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Failed to load Login Success Widget"));
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Login Failed"));
        // �α��� ���� ��, ���� �޽����� �÷��� �������� �����ݴϴ�.
        TSubclassOf<UUserWidget> LoginFailedWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/HitUP/Widgets/Main_Login/WarningMessage/WB_Warning"));
        if (LoginFailedWidgetClass)
        {
            UUserWidget* LoginFailedWidget = CreateWidget<UUserWidget>(GetWorld(), LoginFailedWidgetClass);
            // Ư�� ��ġ�� ������ �߰��մϴ�.
            LoginFailedWidget->AddToViewport();
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Failed to load Login Failed Widget"));
        }
    }
}

void ACGoogleLoginActor::OnGoogleLoginButtonClicked()
{
    if (!bIsLoggedIn)
    {
        // ���� �α��� ��ư�� Ŭ���Ǿ��� ���� �����Դϴ�.
        FString GoogleLoginUrl = "http://hitup.shop:8000/google/login/";

        // ���⼭ ���� ���� �����ϰ� URL�� �߰����ݴϴ�.
        FString RandomValue = GenerateRandomString(10); // ������ ����
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