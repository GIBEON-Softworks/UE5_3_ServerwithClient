// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NetOtherCharacter.generated.h"

class AAIController;

UCLASS()
class SAMPLE_API ANetOtherCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANetOtherCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	inline void SetTargetPos(FVector v)
	{
		Target = v;
	}
	
private:
	void MoveToTarget(float DeltaTime, float Speed);

private:
	bool FirstMove;
	FVector Target;
	AAIController* MyAIController;
};
