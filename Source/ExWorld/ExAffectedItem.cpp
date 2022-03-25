// Fill out your copyright notice in the Description page of Project Settings.


#include "ExAffectedItem.h"
#include "Components/SphereComponent.h"



// Sets default values
AExAffectedItem::AExAffectedItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Affected");

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component

	RootComponent = CollisionComp;


}

// Called when the game starts or when spawned
void AExAffectedItem::BeginPlay()
{
	Super::BeginPlay();
	
	ECollisionChannel CollisionChannel = CollisionComp->GetCollisionObjectType();

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("LOCATION: %s"), *StaticEnum<ECollisionChannel>()->GetNameStringByValue((int64)CollisionChannel)));


}
