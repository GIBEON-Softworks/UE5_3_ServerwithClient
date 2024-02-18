// copyright by Shin ChungSik

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CAsyncLoadingController.generated.h"

/**
 * 위젯으로 로딩창을 구현하고 맵의 환경이 불러와지는 시간 만큼 로딩을 진행합니다.
 * 이외로 이 구성을 통해 로그인에 대한 대기 상태도 기대해봅니다.
 */
UCLASS()
class HITUP_API ACAsyncLoadingController : public APlayerController
{
	GENERATED_BODY()

public:
	/**
	* Add AsyncLevelLoad
	* @param InString AsyncLevelLoad input your Level Reference address(that file address) and input your levelName
	*/
	//UFUNCTION(BlueprintCallable, Category = "AsyncLevelLoading", Meta = (ToolTip = "LevelAsyncCall", DisplayName = "this is can LevelLoad Async"))
	UFUNCTION(BlueprintCallable, Category = "AsyncLevelLoading", Meta = (ToolTip = "LevelAsync Help Loading"))
	void AsyncLevelLoad(const FString& LevleDir, const FString& LevelName ); // level 주소와 level 이름을 불러옵니다.

private:
	void AsyncLevelLoadFinished(const FString LevelName);
};
