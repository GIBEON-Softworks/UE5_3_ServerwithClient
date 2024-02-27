#include "CustomActors/CGoogleLoginActor.h"
#include "Http.h"
#include "Blueprint/UserWidget.h"
#include "Json.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"


ACGoogleLoginActor::ACGoogleLoginActor()
{
    PrimaryActorTick.bCanEverTick = true;

    bIsLoggedIn = false;
    ResponseCode = 0;
    bIsCheckingTableCode = false;
}

void ACGoogleLoginActor::BeginPlay()
{
    Super::BeginPlay();


    GetWorldTimerManager().SetTimer(LoginCheckTimerHandle, this, &ACGoogleLoginActor::CheckTableCodePeriodically, 5.0f, true);
}


void ACGoogleLoginActor::OnGoogleLoginButtonClicked()
{
    if (!bIsLoggedIn)
    {
        FString GoogleLoginUrl = "https://hitup.shop/google/login/";
        RandomValue = GenerateRandomString(10);
        GoogleLoginUrl += RandomValue;

        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, GoogleLoginUrl);
        OpenChromeBrowser(GoogleLoginUrl);
        SendHttpRequest(GoogleLoginUrl, TEXT("GET"), TEXT(""));
    }
}

void ACGoogleLoginActor::OpenChromeBrowser(const FString& URL)
{
    bIsCheckingTableCode = true;
    // 로그 추가: CreateProc 호출
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("FPlatformProcess::CreateProc failed with error code: ") + FString::FromInt(LastError));
    FPlatformProcess::CreateProc(*URL, nullptr, true, false, false, nullptr, 0, nullptr, nullptr);
    LastTableCheckUrl = "https://hitup.shop/google/login/tablecheck/" + RandomValue;
}

void ACGoogleLoginActor::OnBrowserClosed(const FString& URL)
{
    bIsCheckingTableCode = false;
}

void ACGoogleLoginActor::HandleLoginResponse(bool bSuccess, int32 Code)
{
    if (!bSuccess)
    {
        switch (Code)
        {
        case 2000:
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Login Success"));
            break;
        case 4000:
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Login Failed: Bad Request"));
            break;
        case 4400:
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Login Failed: Social Login Info Not Found"));
            break;
        case 4401:
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Login Failed: Incorrect Auth Key"));
            break;
        case 4402:
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Login Failed: Non-Existent Auth Key"));
            break;
        case 4403:
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Login Failed: Auth Key Expired"));
            break;
        default:
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Login Failed: Unknown Error"));
            break;
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Login Failed"));
    }

    if (bSuccess)
    {
        bIsLoggedIn = true;
    }
}

void ACGoogleLoginActor::SendHttpRequest(const FString& URL, const FString& Verb, const FString& Content)
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb(Verb);
    HttpRequest->SetContentAsString(Content);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ACGoogleLoginActor::HandleGoogleLoginResponse);
    HttpRequest->ProcessRequest();
}

void ACGoogleLoginActor::HandleGoogleLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid() && Response->GetResponseCode() == EHttpResponseCodes::Ok)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Login Success"));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Failed to send HTTP request(HandleGoogleLoginResponse)"));
    }
}

void ACGoogleLoginActor::CheckTableCode(const FString& Url, const FString& Content)
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb(TEXT("GET"));
    Request->SetContentAsString(Content);
    Request->OnProcessRequestComplete().BindUObject(this, &ACGoogleLoginActor::HandleTableCodeResponse);
    Request->ProcessRequest();

}

void ACGoogleLoginActor::HandleTableCodeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid() && Response->GetResponseCode() == EHttpResponseCodes::Ok)
    {
        FString ResponseData = Response->GetContentAsString();
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(ResponseData);

        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            int32 Code = JsonObject->GetIntegerField("code");
            if (Code == 2000)
            {
                HandleLoginResponse(true, Code);
            }
            else
            {
                HandleLoginResponse(false, Code);
            }
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("JSON Parsing Error"));
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Failed to send HTTP request(HandleTableCodeResponse)"));
    }

    // 1초마다 다시 tablecheck 요청을 보내도록 함
    GetWorldTimerManager().SetTimerForNextTick([this]() {
        if (bIsCheckingTableCode) {
            CheckTableCode(LastTableCheckUrl, TEXT(""));
        }
    });
}

void ACGoogleLoginActor::CheckTableCodePeriodically()
{
    if (bIsCheckingTableCode)
    {
        CheckTableCode(LastTableCheckUrl, TEXT(""));
    }
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


void ACGoogleLoginActor::Tick(float DeltaTime) // Tick 함수 구현
{
    Super::Tick(DeltaTime);

    // 주기적으로 Tablecheck 확인
    if (bIsCheckingTableCode)
    {
        CheckTableCode(LastTableCheckUrl, TEXT(""));
    }
}