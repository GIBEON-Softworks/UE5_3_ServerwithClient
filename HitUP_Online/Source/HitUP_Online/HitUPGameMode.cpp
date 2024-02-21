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
		// ���� ������ ���Ͽ� �� ������ ������ ������ ��
		StartWidget = CreateWidget<UUserWidget>(GetWorld(), Level1StartWidgetClass);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		ChangeMenuWidget(Level1StartWidgetClass);

		//GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Red, TEXT("���� Level : Lv_Lobby01"));
	}
	else if (GetWorld()->GetMapName() == "UEDPIE_0_Lv_Map01")
	{
		StartWidget = nullptr; //CreateWidget<UUserWidget>(GetWorld(), Level2StartWidgetClass);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

		//GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Red, TEXT("���� Level : Lv_Map01"));
	}
	//ChangeMenuWidget(StartingWidgetClass);
}

void AHitUPGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		// UUserWidget���� �����ϴ� RemoveFromViewport()�� ����ؼ�
		// viewoport�� �ִ� UI�� �����Ѵ�
		CurrentWidget->RemoveFromParent(); //240219 : RemoveFromViewport -> RemoveFromParent ���� viewport�� ���� ����
		CurrentWidget = nullptr;

		GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Blue, TEXT("input ChangeMenuWidget"));
	}

	if (NewWidgetClass != nullptr)
	{
		// �Ű������� ���� NewWidgetClass�� ��ȿ���� Ȯ���ϰ�,

		// CurrentWidget�Լ��� NewWidgetClass�� �����ϰ�
		CurrentWidget = CreateWidget(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			// �� ������ �����
			CurrentWidget->AddToViewport();
		}
	}
}

void AHitUPGameMode::ChangeLevel(const FString& LevelName)
{
	GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Red, TEXT("input ChangeLevel"));

	/*FString NextLevelName = LevelName;*/

	// level ��ȯ �ż��� - �ֽ¿�
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

	// HTTP ��û ��ü ����
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Url);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(JsonData);

	// 1. �ε� â�� ���� �����ش�

	// ��û �Ϸ� �� ȣ��� �ݹ� �Լ� ���� ( ��� )
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AHitUPGameMode::OnHttpRequestComplete);

	// ��û ������
	HttpRequest->ProcessRequest();

	// ��û ������ 
	//if (HttpRequest->ProcessRequest())
	//{
			// ��û ������
			// GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Green, TEXT(" Post Call Web Browser"));
	//}
	//else
	//{
			// ��û ���н�
	//}


}

// HTTP ��û �Ϸ� �� ȣ��Ǵ� �ݹ� �Լ�
void AHitUPGameMode::OnHttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (bSuccess && Response.IsValid())
	{
		// ������ ���������� �޾����� �� ó���ϴ� �ڵ�
		FString ResponseData = Response->GetContentAsString();
		UE_LOG(LogTemp, Warning, TEXT("HTTP Response: %s"), *ResponseData);
		
		// 2. �ε� â ���ָ鼭 ---> ���� �̵�
		if (ResponseData != "")
		{
			// �α��� ���� �����Ͱ� ��� �Ѿ������ Ȯ�� 
			GEngine->AddOnScreenDebugMessage(-10, 10.0f, FColor::Green, ResponseData);
			// 1. �г����� �� �Ѿ�Դٸ�, 
			// 2. 

			ChangeLevel("Lv_Map01");
		}
		else
		{
			// ������ �ȵ�����, 
			// 1. �Ľ��ؼ� ���� �޼����� ��� �Ѵ�
			// 2. 
			GEngine->AddOnScreenDebugMessage(-10, 2.0f, FColor::Green, ResponseData);
		}
	}
	else
	{
		// ��û�� �������� �� ó���ϴ� �ڵ�
		//UE_LOG(LogTemp, Error, TEXT("HTTP Request failed"));
		GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Red, TEXT("HTTP Request failed"));
	}
}

FString AHitUPGameMode::HashString(const FString& InputString)
{	
	return "";
}



// 192.168.0.118



