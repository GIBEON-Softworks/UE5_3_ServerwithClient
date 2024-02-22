#include "Weapons/CWeaponActor.h"
#include "Components/CapsuleComponent.h"


// Sets default values
ACWeaponActor::ACWeaponActor()
{
    // Set up collision component
    CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitCapsuleSize(30.0f, 70.0f); // 기본 캡슐 크기 (반지름, 반 높이)
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACWeaponActor::OnOverlapBegin);
    CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ACWeaponActor::OnOverlapEnd);
    RootComponent = CollisionComponent;


    // Set up skeletal mesh component
    //SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    //SkeletalMeshComponent->SetupAttachment(RootComponent); // Attach to root component


    // Set up default values for other properties
    AttackMontage = nullptr;

}


// Called when the game starts or when spawned
void ACWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
    // Configure perk functions
    ConfigurePerkFunctions();
}

// Called when the CollisionComponent overlaps another actor
void ACWeaponActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Implement your logic when overlap begins
}

// Called when the CollisionComponent stops overlapping another actor
void ACWeaponActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // Implement your logic when overlap ends
}


// Configure functions by perk for the weapon
void ACWeaponActor::ConfigurePerkFunctions()
{
    // Implement your perk-specific configuration here
}


// Function to deal damage to the OtherActor
void ACWeaponActor::DealDamage(AActor* OtherActor)
{
    // Implement your damage dealing logic here
}
