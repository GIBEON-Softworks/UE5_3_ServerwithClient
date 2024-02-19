// copyright by Shin ChungSik

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAsyncActor.generated.h"

UCLASS()
class HITUP_API ACAsyncActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACAsyncActor();

public:
	/**
	* Add AsyncLevelLoad
	* @param InString AsyncLevelLoad input your Level Reference address(that file address) and input your levelName
	*/
	//UFUNCTION(BlueprintCallable, Category = "AsyncLevelLoading", Meta = (ToolTip = "LevelAsyncCall", DisplayName = "this is can LevelLoad Async"))
	UFUNCTION(BlueprintCallable, Category = "AsyncLevelLoading", Meta = (ToolTip = "LevelAsync Help Loading"))
	void AsyncLevelLoad(const FString& LevleDir, const FString& LevelName); // level 주소와 level 이름을 불러옵니다.

private:
	void AsyncLevelLoadFinished(const FString LevelName);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
