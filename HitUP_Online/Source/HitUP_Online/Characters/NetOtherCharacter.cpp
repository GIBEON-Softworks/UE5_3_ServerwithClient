// Fill out your copyright notice in the Description page of Project Settings.


#include "NetOtherCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
ANetOtherCharacter::ANetOtherCharacter()
	:FirstMove(true)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 0;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
}

// Called when the game starts or when spawned
void ANetOtherCharacter::BeginPlay()
{
	Super::BeginPlay();	

	// AIController Spawn
	MyAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, this->GetActorLocation(), this->GetActorRotation());

	if (MyAIController)
	{
		// 스폰한 AIController로 Pawn을 제어하도록 설정합니다.
		MyAIController->Possess(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn AI Controller."));
	}
}

// Called every frame
void ANetOtherCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveToTarget(DeltaTime,500);

}

void ANetOtherCharacter::MoveToTarget(float DeltaTime, float Speed)
{
	if (FirstMove == true && Target.Size() > 0)
	{
		SetActorLocation(Target);
		FirstMove = false;
		return;
	}

	// 현재 위치와 목표 위치 사이의 벡터 계산
	FVector CurrentLocation = GetActorLocation();
	FRotator CurrentRotation = GetActorRotation();

	double length = (Target - CurrentLocation).Size();

	if (length < 5)
	{
		return;
	}

	FVector Direction = (Target - CurrentLocation).GetSafeNormal();
	FVector NewLocation = GetActorLocation() + Direction * Speed * DeltaTime;

	// 타겟 방향으로의 회전값 계산
	FRotator DirectionRotation = Direction.Rotation();

	// Yaw 값만 타겟의 회전값으로 설정하고, Pitch와 Roll은 현재 캐릭터의 값을 유지
	FRotator TargetRotation = FRotator(CurrentRotation.Pitch, DirectionRotation.Yaw, CurrentRotation.Roll);
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 100);


	SetActorRotation(NewRotation);
	SetActorLocation(NewLocation);
}



