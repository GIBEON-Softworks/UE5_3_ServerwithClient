// copyright by Shin ChungSik


#include "CustomActors/CAsyncActor.h"
#include "UObject/UObjectGlobals.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACAsyncActor::ACAsyncActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


void ACAsyncActor::AsyncLevelLoad(const FString& LevelDir, const FString& LevelName)
{
	// LoadPackageAsync �Լ��� ����Ͽ� �񵿱������� ������ �ε��մϴ�.
	// LevelDir�� LevelName�� ���ļ� �ε��� ������ ��θ� �����մϴ�.
	LoadPackageAsync(LevelDir + LevelName,
		// CreateLambda �Լ��� ����Ͽ� ���� �Լ��� �����մϴ�.
		FLoadPackageAsyncDelegate::CreateLambda([this, LevelName](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
			{
				// �ε尡 �������� ���
				if (Result == EAsyncLoadingResult::Succeeded)
				{
					// AsyncLevelLoadFinished �Լ��� ȣ���Ͽ� �ε� �ϷḦ ó���մϴ�.
					AsyncLevelLoadFinished(LevelName);
				}
			}),

		// �ε� �ɼ��� �����մϴ�. ���⼭�� 0�� ����Ͽ� �⺻ �ɼ��� �����մϴ�.
		0,
		// PKG_ContainsMap �ɼ��� ����Ͽ� �ε��� ��Ű���� ���� �����ϴ��� ���θ� �����մϴ�.
		PKG_ContainsMap
	);
}

void ACAsyncActor::AsyncLevelLoadFinished(const FString LevelName)
{
	UGameplayStatics::OpenLevel(this, FName(*LevelName));
}


// Called when the game starts or when spawned
void ACAsyncActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACAsyncActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

