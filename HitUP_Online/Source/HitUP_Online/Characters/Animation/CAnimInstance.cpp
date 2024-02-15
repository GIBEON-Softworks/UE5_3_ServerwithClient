// Fill out your copyright notice in the Description page of Project Settings.


#include "CAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	//현재 애니메이션 인스턴스가 속한 블루프린트 소유자의 참조를 가져온다.
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter == nullptr) return; // 소유 캐릭터가 없을 경우

}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (OwnerCharacter == nullptr) return;

	// Movement component 에서 속도 가져오기

	// Check if the character should move based on ground speed and acceleration
	const float Threshold = 10.0f; // 임계값 설정
	const FVector CurrentVelocity = OwnerCharacter->GetCharacterMovement()->Velocity; //블루프린트 작성법과 비슷
	const bool bHasAcceleration = Velocity.SizeSquared() > SMALL_NUMBER;

	// 캐릭터의 지면 속도 계산 (X와 Y축만 사용)
	FVector GroundVelocity = FVector(CurrentVelocity.X, CurrentVelocity.Y, 0.0f);
	float GroundSpeed = GroundVelocity.Size();


	// Update speed
	Speed = CurrentVelocity.Size();
	//Speed = OwnerCharacter->GetVelocity().Size2D();

	// 방향 업데이트 (yaw)
	FRotator ControlRotation = OwnerCharacter->GetControlRotation();
	FRotator CharacterRotation = OwnerCharacter->GetActorRotation();

	Speed = OwnerCharacter->GetVelocity().Size();
	//Direction = OwnerCharacter->GetControlRotation().Yaw;
	Direction = FRotator::NormalizeAxis(ControlRotation.Yaw - CharacterRotation.Yaw);

	

	// 캐릭터가 떨어지고 있는지 확인
	bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();

	// Check if character is moving
	bMoving = Speed > 0;
	bMoving = GroundSpeed > Threshold && bHasAcceleration;


	//업데이트 Velocity
	Velocity = OwnerCharacter->GetVelocity();

}

void UCAnimInstance::UpdateAnimationProperties()
{
}
