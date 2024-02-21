// copyright by Choi SeungWoo

#include "HitUPGameMode.h"

#include "Engine.h"
#include "UObject/ConstructorHelpers.h"
#include <Blueprint/UserWidget.h>

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

#include "Misc/SecureHash.h"
#include "Misc/Base64.h"

#include "GenericPlatform/GenericPlatformMisc.h"


AHitUPGameMode::AHitUPGameMode()
{
}

void AHitUPGameMode::BeginPlay()
{
	Super::BeginPlay();

	FString current = GetWorld()->GetMapName();
	//GEngine->AddOnScreenDebugMessage(-3, 30.0f, FColor::Red, current);

	UUserWidget* StartWidget = nullptr;

	// UEDPIE_0_
	if (GetWorld()->GetMapName() == "UEDPIE_0_Lv_Lobby01")
	{
		// 현재 레벨을 비교하여 각 레벨의 설정을 맞춰준 다
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
		CurrentWidget->RemoveFromParent(); //240219 : RemoveFromViewport -> RemoveFromParent 변경 viewport는 이전 사항
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

void AHitUPGameMode::CallLogin(const FString& Id, const FString& Password)
{
	//FString Url = TEXT("http://192.168.0.118:8000/jun/test/7");
	FString Url = TEXT("http://hitup.shop:8000/api/signin");

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(TEXT("id"), Id);
	JsonObject->SetStringField(TEXT("password"), Password);

	FString JsonData;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonData);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	// HTTP 요청 객체 생성
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Url);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(JsonData);

	// 1. 로딩 창을 먼저 보여준다

	// 요청 완료 후 호출될 콜백 함수 설정 ( 대기 )
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AHitUPGameMode::OnHttpRequestComplete);

	// 요청 보내기
	HttpRequest->ProcessRequest();

	// 요청 보내기 
	//if (HttpRequest->ProcessRequest())
	//{
			// 요청 성공시
			// GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Green, TEXT(" Post Call Web Browser"));
	//}
	//else
	//{
			// 요청 실패시
	//}


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
		if (ResponseData != "")
		{
			// 로그인 성공 데이터가 어떻게 넘어오는지 확인 
			GEngine->AddOnScreenDebugMessage(-10, 10.0f, FColor::Green, ResponseData);
			// 1. 닉네임이 잘 넘어왔다면, 
			// 2. 

			ChangeLevel("Lv_Map01");
		}
		else
		{
			// 연결이 안됐을때, 
			// 1. 파싱해서 오류 메세지를 출력 한다
			// 2. 
			GEngine->AddOnScreenDebugMessage(-10, 2.0f, FColor::Green, ResponseData);
		}
	}
	else
	{
		// 요청이 실패했을 때 처리하는 코드
		//UE_LOG(LogTemp, Error, TEXT("HTTP Request failed"));
		GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Red, TEXT("HTTP Request failed"));
	}
}

FString AHitUPGameMode::HashString(const FString& InputString)
{	
	return "";
}



// 192.168.0.118



