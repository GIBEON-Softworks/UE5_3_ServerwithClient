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
	// LoadPackageAsync 함수를 사용하여 비동기적으로 레벨을 로드합니다.
	// LevelDir과 LevelName을 합쳐서 로드할 레벨의 경로를 지정합니다.
	LoadPackageAsync(LevelDir + LevelName,
		// CreateLambda 함수를 사용하여 람다 함수를 생성합니다.
		FLoadPackageAsyncDelegate::CreateLambda([this, LevelName](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
			{
				// 로드가 성공했을 경우
				if (Result == EAsyncLoadingResult::Succeeded)
				{
					// AsyncLevelLoadFinished 함수를 호출하여 로드 완료를 처리합니다.
					AsyncLevelLoadFinished(LevelName);
				}
			}),

		// 로드 옵션을 설정합니다. 여기서는 0을 사용하여 기본 옵션을 설정합니다.
		0,
		// PKG_ContainsMap 옵션을 사용하여 로드할 패키지가 맵을 포함하는지 여부를 설정합니다.
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

