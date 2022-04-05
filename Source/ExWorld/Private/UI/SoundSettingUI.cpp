// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SoundSettingUI.h"

#include "Components/Slider.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

#include "SoundSettingSaveGame.h"


USoundSettingUI::USoundSettingUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void USoundSettingUI::NativeConstruct()
{
	Super::NativeConstruct();
	BtnSave->OnPressed.AddDynamic(this, &USoundSettingUI::OnSaveButtonPressed);
	Btnload->OnPressed.AddDynamic(this, &USoundSettingUI::OnLoadButtonPressed);
	BtnClose->OnPressed.AddDynamic(this, &USoundSettingUI::OnCloseButtonPressed);

}

void USoundSettingUI::NativeDestruct()
{
	BtnSave->OnPressed.RemoveDynamic(this, &USoundSettingUI::OnSaveButtonPressed);
	Btnload->OnPressed.RemoveDynamic(this, &USoundSettingUI::OnLoadButtonPressed);
	BtnClose->OnPressed.RemoveDynamic(this, &USoundSettingUI::OnCloseButtonPressed);

	Super::NativeDestruct();
}

void USoundSettingUI::OnSaveButtonPressed()
{

	USoundSettingSaveGame* SaveGameInstance = Cast<USoundSettingSaveGame>(UGameplayStatics::CreateSaveGameObject(USoundSettingSaveGame::StaticClass()));
	SaveGameInstance->SouldSliderData = SouldSliderData->GetValue();
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);

}

void USoundSettingUI::OnLoadButtonPressed()
{
	USoundSettingSaveGame* LoadGameInstance = Cast<USoundSettingSaveGame>(UGameplayStatics::CreateSaveGameObject(USoundSettingSaveGame::StaticClass()));
	LoadGameInstance = Cast<USoundSettingSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
	SouldSliderData->SetValue(LoadGameInstance->SouldSliderData);

}

void USoundSettingUI::OnCloseButtonPressed()
{
	RemoveFromParent();

	APlayerController* Controller = UGameplayStatics::GetPlayerController(GWorld->GetWorld(), 0);
	Controller->SetShowMouseCursor(false);
	Controller->SetInputMode(FInputModeGameOnly());

}

