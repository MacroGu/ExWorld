// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SoundSettingSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class EXWORLD_API USoundSettingSaveGame : public USaveGame
{
	GENERATED_BODY()
	

public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
	float SouldSliderData;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;


	USoundSettingSaveGame();

};
