// copyright by Shin ChungSik


#include "Controllers/CAsyncLoadingController.h"
#include "UObject/UObjectGlobals.h"
#include "Kismet/GameplayStatics.h"

void ACAsyncLoadingController::AsyncLevelLoad(const FString& LevelDir, const FString& LevelName)
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

void ACAsyncLoadingController::AsyncLevelLoadFinished(const FString LevelName)
{
	UGameplayStatics::OpenLevel(this, FName(*LevelName));
}
