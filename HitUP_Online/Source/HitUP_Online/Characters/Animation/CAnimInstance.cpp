// Fill out your copyright notice in the Description page of Project Settings.


#include "CAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter == nullptr) return;

}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (OwnerCharacter == nullptr) return;

	// Update speed
	Speed = OwnerCharacter->GetVelocity().Size2D();

	// Update direction (yaw)
	FRotator ControlRotation = OwnerCharacter->GetControlRotation();
	FRotator CharacterRotation = OwnerCharacter->GetActorRotation();

	Speed = OwnerCharacter->GetVelocity().Size();
	//Direction = OwnerCharacter->GetControlRotation().Yaw;
	Direction = FRotator::NormalizeAxis(ControlRotation.Yaw - CharacterRotation.Yaw);
	bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();



	// Check if character is moving
	bMoving = Speed > 0;

}

void UCAnimInstance::UpdateAnimationProperties()
{
}
