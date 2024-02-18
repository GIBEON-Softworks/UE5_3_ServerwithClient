// copyright by Shin ChungSik

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CAsyncLoadingController.generated.h"

/**
 * �������� �ε�â�� �����ϰ� ���� ȯ���� �ҷ������� �ð� ��ŭ �ε��� �����մϴ�.
 * �ܷ̿� �� ������ ���� �α��ο� ���� ��� ���µ� ����غ��ϴ�.
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
	void AsyncLevelLoad(const FString& LevleDir, const FString& LevelName ); // level �ּҿ� level �̸��� �ҷ��ɴϴ�.

private:
	void AsyncLevelLoadFinished(const FString LevelName);
};
