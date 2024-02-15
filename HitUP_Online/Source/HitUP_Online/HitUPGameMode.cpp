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
	// ������ �ޱ�
}







