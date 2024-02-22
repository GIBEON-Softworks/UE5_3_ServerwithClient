// Weapon 관련한 로직 구성 작성자 : 신충식

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWeaponActor.generated.h"

UCLASS()
class HITUP_API ACWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// SphereCollision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class UCapsuleComponent* CollisionComponent;

	// Animation Montage 추가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	class UAnimMontage* AttackMontage;

	// 스켈레탈 메시나 정적 메시
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	//class USkeletalMeshComponent* SkeletalMeshComponent;

	// Damage handling
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void DealDamage(AActor* OtherActor);

protected:
	// Called when the CollisionComponent overlaps another actor
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called when the CollisionComponent stops overlapping another actor
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Game")
	TArray<class UShapeComponent*> Collisions;

private:
	// Configure functions by perk for the weapon
	void ConfigurePerkFunctions();

};
