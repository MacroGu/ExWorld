// Fill out your copyright notice in the Description page of Project Settings.


#include "ExAffectedItem.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "UI/ExCharacterStatusBarWidget.h"
#include "Kismet/GameplayStatics.h"

#include "ExPlayerController.h"


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

	UIExCharacterStatusBarComponent = CreateDefaultSubobject<UWidgetComponent>(FName("UIExCharacterStatusBarComponent"));
	UIExCharacterStatusBarComponent->SetupAttachment(RootComponent);
	UIExCharacterStatusBarComponent->SetRelativeLocation(FVector(0, 0, 120));
	UIExCharacterStatusBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	UIExCharacterStatusBarComponent->SetDrawSize(FVector2D(500, 500));



}

void AExAffectedItem::InitWidgetComponent()
{
	// Only create once
	if (UIExCharacterStatusBarWidget)
	{
		return;
	}

	ECollisionChannel CollisionChannel = CollisionComp->GetCollisionObjectType();
	if (ECollisionChannel::ECC_Pawn != CollisionChannel)
	{
		return;
	}

	if (!UIExCharacterStatusBarClass)
	{
		return;
	}

	AExPlayerController* PC = Cast<AExPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PC)
	{
		return;
	}

	UIExCharacterStatusBarWidget = CreateWidget<UExCharacterStatusBarWidget>(PC, UIExCharacterStatusBarClass);
	if (!UIExCharacterStatusBarWidget || !UIExCharacterStatusBarComponent)
	{
		return;
	}

	UIExCharacterStatusBarComponent->SetWidget(UIExCharacterStatusBarWidget);

	// Setup the floating status bar
	UIExCharacterStatusBarWidget->SetHealthPercentage(CurrentHP);
}

// Called when the game starts or when spawned
void AExAffectedItem::BeginPlay()
{
	Super::BeginPlay();
	
	ECollisionChannel CollisionChannel = CollisionComp->GetCollisionObjectType();

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("LOCATION: %s"), *StaticEnum<ECollisionChannel>()->GetNameStringByValue((int64)CollisionChannel)));

	InitWidgetComponent();
}

void AExAffectedItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AExAffectedItem, CurrentHP);

}

void AExAffectedItem::OnRep_CurrentHP()
{
	UIExCharacterStatusBarWidget->SetHealthPercentage(CurrentHP);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("HP %d"), CurrentHP));

}

void AExAffectedItem::ChangeHealth_Implementation(int32 NewValue)
{
	CurrentHP = CurrentHP - NewValue;
}

bool AExAffectedItem::ChangeHealth_Validate(int32 NewValue)
{
	return true;
}
