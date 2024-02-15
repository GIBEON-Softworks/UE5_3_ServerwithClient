// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HITUP_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float Pitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	FVector Velocity; //벡터변수

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bMoving;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	// 애니메이션을 갱신하는 함수를 선언합니다.
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void UpdateAnimationProperties();

private:
	class ACharacter* OwnerCharacter;


};
