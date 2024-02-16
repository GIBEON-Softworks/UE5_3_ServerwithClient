#include "HitUPGameMode.h"

#include "Engine.h"
#include "UObject/ConstructorHelpers.h"
#include <Blueprint/UserWidget.h>

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"


AHitUPGameMode::AHitUPGameMode()
{
}

void AHitUPGameMode::BeginPlay()
{
	Super::BeginPlay();

	FString current = GetWorld()->GetMapName();
	GEngine->AddOnScreenDebugMessage(-3, 30.0f, FColor::Red, current);

	UUserWidget* StartWidget = nullptr;

	// UEDPIE_0_
	if (GetWorld()->GetMapName() == "UEDPIE_0_Lv_Lobby01")
	{
		StartWidget = CreateWidget<UUserWidget>(GetWorld(), Level1StartWidgetClass);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		ChangeMenuWidget(Level1StartWidgetClass);

		//GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Red, TEXT("현재 Level : Lv_Lobby01"));
	}
	else if (GetWorld()->GetMapName() == "UEDPIE_0_Lv_Map01")
	{
		StartWidget = nullptr; //CreateWidget<UUserWidget>(GetWorld(), Level2StartWidgetClass);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

		//GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Red, TEXT("현재 Level : Lv_Map01"));
	}
	//ChangeMenuWidget(StartingWidgetClass);
}

void AHitUPGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		// UUserWidget에서 제공하는 RemoveFromViewport()를 사용해서
		// viewoport에 있는 UI를 제거한다
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;

		GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Blue, TEXT("input ChangeMenuWidget"));
	}

	if (NewWidgetClass != nullptr)
	{
		// 매개변수로 받은 NewWidgetClass가 유효한지 확인하고, 

		// CurrentWidget함수로 NewWidgetClass를 저장하고 
		CurrentWidget = CreateWidget(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			// 새 위젯을 만든다
			CurrentWidget->AddToViewport();
		}
	}
}

void AHitUPGameMode::ChangeLevel(const FString& LevelName)
{
	GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Red, TEXT("input ChangeLevel"));

	/*FString NextLevelName = LevelName;*/

	// level 전환 매서드 - 최승우 
	UGameplayStatics::OpenLevel(GEngine->GetWorld(), FName(*LevelName));
}

void AHitUPGameMode::CalledWeb()
{

}

void AHitUPGameMode::CallLogin()
{
	FString Url = TEXT("http://192.168.0.118:8000/jun/test/7");

	// HTTP 요청 객체 생성
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Url);
	
	GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Green, TEXT(" Post Call Web Browser"));

	// 1. 로딩 창을 먼저 보여준다


	// 요청 완료 후 호출될 콜백 함수 설정 ( 대기 )
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AHitUPGameMode::OnHttpRequestComplete);

	// 요청 보내기
	HttpRequest->ProcessRequest();

}

// HTTP 요청 완료 시 호출되는 콜백 함수
void AHitUPGameMode::OnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (bSuccess && Response.IsValid())
	{
		// 응답이 성공적으로 받아졌을 때 처리하는 코드
		FString ResponseData = Response->GetContentAsString();
		UE_LOG(LogTemp, Warning, TEXT("HTTP Response: %s"), *ResponseData);
		
		// 2. 로딩 창 꺼주면서 ---> 레벨 이동
		if (ResponseData == "")
		{
			GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Green, ResponseData);

			ChangeLevel("Lv_Lobby01");
		}
	}
	else
	{
		// 요청이 실패했을 때 처리하는 코드
		UE_LOG(LogTemp, Error, TEXT("HTTP Request failed"));
	}
}


// 192.168.0.118



