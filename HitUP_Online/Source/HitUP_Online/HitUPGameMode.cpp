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
		CurrentWidget->RemoveFromViewport();
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

void AHitUPGameMode::CallLogin()
{
	FString Url = TEXT("http://192.168.0.118:8000/jun/test/7");

	// HTTP ��û ��ü ����
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Url);
	
	GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Green, TEXT(" Post Call Web Browser"));

	// 1. �ε� â�� ���� �����ش�


	// ��û �Ϸ� �� ȣ��� �ݹ� �Լ� ���� ( ��� )
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AHitUPGameMode::OnHttpRequestComplete);

	// ��û ������
	HttpRequest->ProcessRequest();

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
		if (ResponseData == "")
		{
			GEngine->AddOnScreenDebugMessage(-3, 2.0f, FColor::Green, ResponseData);

			ChangeLevel("Lv_Lobby01");
		}
	}
	else
	{
		// ��û�� �������� �� ó���ϴ� �ڵ�
		UE_LOG(LogTemp, Error, TEXT("HTTP Request failed"));
	}
}


// 192.168.0.118



