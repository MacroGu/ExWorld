// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExWorldCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Net/UnrealNetwork.h"

#include "ExPlayerController.h"
#include "ExWorldProjectile.h"


//////////////////////////////////////////////////////////////////////////
// AExWorldCharacter

AExWorldCharacter::AExWorldCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	bIsSelfPlayingAbilityAnimation = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CoolDownTimeForSpellSkill = 10;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AExWorldCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AExWorldCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AExWorldCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AExWorldCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AExWorldCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AExWorldCharacter::OnSpellSkill);

}

void AExWorldCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AExWorldCharacter, LastSpellTime);

}

void AExWorldCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AExWorldCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AExWorldCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AExWorldCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AExWorldCharacter::OnSpellSkill()
{
	// client check if can spell skill
	int64 LeftTime = FDateTime::Now().ToUnixTimestamp() - LastSpellTime;
	if (LeftTime > CoolDownTimeForSpellSkill)
	{
		ReqSpellAbility();
		bIsSelfPlayingAbilityAnimation = true;
		PlaySpellAbilityAnimation();
		return;
	}

	AExPlayerController* PC = Cast<AExPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PC))
	{
		return;
	}

	PC->ShowTimeLimitation(CoolDownTimeForSpellSkill - LeftTime);

}

void AExWorldCharacter::PlaySpellAbilityAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		PlayAnimMontage(SpellSkillAnimation, 1.f);
	}
}

void AExWorldCharacter::ReqSpellAbility_Implementation()
{
	// server check if can spell ability
	int64 CurrentTime = FDateTime::Now().ToUnixTimestamp();
	if (CurrentTime - LastSpellTime < CoolDownTimeForSpellSkill)
	{
		return;
	}

	LastSpellTime = CurrentTime;

	MulticastClientSpellAbility();
}

void AExWorldCharacter::MulticastClientSpellAbility_Implementation()
{
	if (bIsSelfPlayingAbilityAnimation)
	{
		return;
	}

	PlaySpellAbilityAnimation();
}

void AExWorldCharacter::ReqSpawnProjectile_Implementation()
{
	FActorSpawnParameters params;
	params.Owner = this;
	params.Instigator = this;

	GetWorld()->SpawnActor<AExWorldProjectile>(ProjectileClass, GetActorLocation(), GetActorRotation(), params);
}

void AExWorldCharacter::StartSpawnProjectile()
{
	
	ReqSpawnProjectile();
	bIsSelfPlayingAbilityAnimation = false;

}

void AExWorldCharacter::OnRep_LastSpellTime()
{

}
