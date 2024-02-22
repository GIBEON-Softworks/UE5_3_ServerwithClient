// Weapon ������ ���� ���� �ۼ��� : �����

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

	// Animation Montage �߰�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	class UAnimMontage* AttackMontage;

	// ���̷�Ż �޽ó� ���� �޽�
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
