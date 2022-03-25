// Fill out your copyright notice in the Description page of Project Settings.


#include "ExWorldProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

#include "ExPlayerController.h"




// Sets default values
AExWorldProjectile::AExWorldProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AExWorldProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
	bIsAOE = false;

}

void AExWorldProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* owner = GetOwner();

	if (owner == OtherActor)
	{
		return;
	}
	
	AExAffectedItem* AfftectedItem = Cast<AExAffectedItem>(OtherActor);
	if (!IsValid(AfftectedItem))
	{
		return;
	}

	AExPlayerController* PC = Cast<AExPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PC)
	{
		return;
	}

	FEffectData EffectData;
	EObjectType ObjectType;
	ECollisionChannel CollisionChannel = AfftectedItem->GetCollisionComp()->GetCollisionObjectType();
	if (ECollisionChannel::ECC_Destructible == CollisionChannel)
	{
		ObjectType = EObjectType::Destructible;
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ECC_Destructible"));
	}
	else if (ECollisionChannel::ECC_WorldStatic == CollisionChannel)
	{
		ObjectType = EObjectType::WorldStatic;
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ECC_WorldStatic"));
	}
	else if (ECollisionChannel::ECC_Pawn == CollisionChannel)
	{
		ObjectType = EObjectType::Pawn;
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ECC_Pawn"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("nothing"));
	}

	if (!PC->GetEffectDataByObjectType(ObjectType, EffectData))
	{
		return;
	}

	PC->OnApplyEffect(OtherActor, EffectData);
	
	if (!bIsAOE)
	{
		Destroy();
	}


}