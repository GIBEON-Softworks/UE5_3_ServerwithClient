// Copyright Epic Games, Inc. All Rights Reserved.

#include "HitUPGameMode.h"
#include "Characters/CSampleCharacter.h"
#include "UObject/ConstructorHelpers.h"


AHitUPGameMode::AHitUPGameMode()
{
	// set default pawn class to our Blueprinted character
	///Script/Engine.Blueprint'/Game/HitUP/Characters/BPC_SampleCharacter.BPC_SampleCharacter'
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/HitUP/Characters/BPC_SampleCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
