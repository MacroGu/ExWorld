// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SoundSettingUI.generated.h"



class USlider;
class UButton;


/**
 * 
 */
UCLASS()
class EXWORLD_API USoundSettingUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USoundSettingUI(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;


	UPROPERTY(Meta = (BindWidget))
	USlider* SouldSliderData = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UButton* BtnSave = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UButton* Btnload = nullptr;

	UPROPERTY(Meta = (BindWidget))
	UButton* BtnClose = nullptr;


protected:

	UFUNCTION()
	void OnSaveButtonPressed();
	UFUNCTION()
	void OnLoadButtonPressed();
	UFUNCTION()
	void OnCloseButtonPressed();


};
