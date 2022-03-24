// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExWorldGameMode.h"
#include "ExWorldCharacter.h"
#include "UObject/ConstructorHelpers.h"

AExWorldGameMode::AExWorldGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
