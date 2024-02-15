#include "HitUPGameMode.h"

#include "Engine.h"
#include "UObject/ConstructorHelpers.h"
#include <Blueprint/UserWidget.h>

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
	// 브라우저 달기
}







