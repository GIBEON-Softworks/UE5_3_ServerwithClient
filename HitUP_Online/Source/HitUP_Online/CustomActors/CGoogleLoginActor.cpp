/**
* Producer : ShinChungSik
* Date of production : 240221 -> 240228(Fin)
* this Game function : Google Login Actors
*/
#include "CustomActors/CGoogleLoginActor.h"
#include "Http.h"
#include "Blueprint/UserWidget.h"
#include "Json.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "Kismet/GameplayStatics.h"

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

// 버튼 클릭시
void ACGoogleLoginActor::OnGoogleLoginButtonClicked()
{
    if (!bIsLoggedIn)
    {
        GoogleLoginUrl = "https://hitup.shop/google/login/";
        RandomValue = GenerateRandomString(10); // 난수 생성
        GoogleLoginUrl += RandomValue;

        // 브라우저를 열고 로그인 페이지를 띄움
        SendHttpRequest(GoogleLoginUrl, TEXT("GET"), TEXT("")); //데이터 요청
        OpenChromeBrowser(GoogleLoginUrl);

        LastTableCheckUrl = "https://hitup.shop/google/login/tablecheck/" + RandomValue;
    }
}

void ACGoogleLoginActor::OpenChromeBrowser(const FString& URL)
{
    bIsCheckingTableCode = true;

    // 로그 추가: LaunchURL 호출
    FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);
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
            ///Script/Engine.World'/Game/HitUP/Levels/Lv_MainGame_01.Lv_MainGame_01'
            MoveToNewLevel("Lv_MainGame_01");
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
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Opened"));
        CheckTableCodePeriodically(); // 테이블 코드 확인을 시작
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

        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Response(HandleTableCodeResponse) : %s"), *ResponseData));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("URI : %s"), *LastTableCheckUrl));
        //UE_LOG(LogTemp, Log, TEXT("Response(HandleTableCodeResponse) : %s"), *ResponseData); // Output Log에 로그 출력
        UE_LOG(LogTemp, Log, TEXT("URI : %s"), *LastTableCheckUrl); // Output Log에 로그 출력


        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            int32 Code = JsonObject->GetIntegerField("code");
            if (Code == 2000)
            {
                //TODO : Code 값만 넘겨주는걸로 하자
                HandleLoginResponse(false, Code);
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
}

void ACGoogleLoginActor::CheckTableCodePeriodically()
{
    // 몇 초 마다 해당 테이블 체크를 할 것인가? (내부 함수)
    GetWorldTimerManager().SetTimer(LoginCheckTimerHandle, this, &ACGoogleLoginActor::HandleCheckTableCodeTimer, 1.0f, true);
}

// 새로운 함수 추가: 타이머에 사용될 함수
void ACGoogleLoginActor::HandleCheckTableCodeTimer()
{
    // 여기에 테이블 코드를 확인하는 로직을 추가합니다.
    //FString TableCheckUrl = "https://hitup.shop/google/login/tablecheck/" + RandomValue;
    CheckTableCode(LastTableCheckUrl, TEXT(""));
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

    //// 주기적으로 Tablecheck 확인
    //if (bIsCheckingTableCode)
    //{
    //    CheckTableCode(LastTableCheckUrl, TEXT(""));
    //}
}


// 레벨 이동 함수
void ACGoogleLoginActor::MoveToNewLevel(const FString& LevelName)
{
    // 이동할 레벨의 경로를 설정합니다. 여기서 "NewLevel"은 이동할 레벨의 이름입니다.
    //FString LevelName = TEXT("NewLevel");
    GetWorldTimerManager().ClearTimer(LoginCheckTimerHandle);
    bIsCheckingTableCode = false;

    // 레벨 이동을 수행합니다.
    UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName), true);
}