// Fill out your copyright notice in the Description page of Project Settings.


#include "CAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	//���� �ִϸ��̼� �ν��Ͻ��� ���� �������Ʈ �������� ������ �����´�.
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter == nullptr) return; // ���� ĳ���Ͱ� ���� ���

}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (OwnerCharacter == nullptr) return;

	// Movement component ���� �ӵ� ��������

	// Check if the character should move based on ground speed and acceleration
	const float Threshold = 10.0f; // �Ӱ谪 ����
	const FVector CurrentVelocity = OwnerCharacter->GetCharacterMovement()->Velocity; //�������Ʈ �ۼ����� ���
	const bool bHasAcceleration = Velocity.SizeSquared() > SMALL_NUMBER;

	// ĳ������ ���� �ӵ� ��� (X�� Y�ุ ���)
	FVector GroundVelocity = FVector(CurrentVelocity.X, CurrentVelocity.Y, 0.0f);
	float GroundSpeed = GroundVelocity.Size();


	// Update speed
	Speed = CurrentVelocity.Size();
	//Speed = OwnerCharacter->GetVelocity().Size2D();

	// ���� ������Ʈ (yaw)
	FRotator ControlRotation = OwnerCharacter->GetControlRotation();
	FRotator CharacterRotation = OwnerCharacter->GetActorRotation();

	Speed = OwnerCharacter->GetVelocity().Size();
	//Direction = OwnerCharacter->GetControlRotation().Yaw;
	Direction = FRotator::NormalizeAxis(ControlRotation.Yaw - CharacterRotation.Yaw);

	

	// ĳ���Ͱ� �������� �ִ��� Ȯ��
	bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();

	// Check if character is moving
	bMoving = Speed > 0;
	bMoving = GroundSpeed > Threshold && bHasAcceleration;


	//������Ʈ Velocity
	Velocity = OwnerCharacter->GetVelocity();

}

void UCAnimInstance::UpdateAnimationProperties()
{
}
