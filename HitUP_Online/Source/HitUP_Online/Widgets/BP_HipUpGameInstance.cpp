// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/BP_HipUpGameInstance.h"



UBP_HipUpGameInstance::UBP_HipUpGameInstance()
{

}

void UBP_HipUpGameInstance::saveToken(const FString& newToken)
{
	token = newToken;

	UE_LOG(LogTemp, Warning, TEXT("Token data saved : %s"), *token);
}

FString UBP_HipUpGameInstance::LoadData()
{
	return token;
}
