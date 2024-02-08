// Copyright Epic Games, Inc. All Rights Reserved.

#include "HitUPGameMode.h"
#include "Characters/CSampleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHitUPGameMode::AHitUPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/MainChar"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
