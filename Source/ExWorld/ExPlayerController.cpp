// Fill out your copyright notice in the Description page of Project Settings.


#include "ExPlayerController.h"


void AExPlayerController::BeginPlay()
{
	UDataTable* pDataTable = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/Data/DT_AffectData.DT_AffectData'"));
	
	if (!pDataTable)
	{
		return;
	}

	for (auto it : pDataTable->GetRowMap())
	{
		FAffectDataTable* pRow = (FAffectDataTable*)it.Value;
		ConfigureData.Add(pRow->AffectType, pRow->AffectData);
	}

}

bool AExPlayerController::GetEffectDataByObjectType(const EObjectType ObjectType, FEffectData& EffectData)
{
	if (!ConfigureData.Contains(ObjectType))
	{
		return false;
	}

	EffectData = *ConfigureData.Find(ObjectType);

	return true;
}

void AExPlayerController::OnApplyEffect(AActor* AffectedActor, const FEffectData& EffectData)
{

	AExAffectedItem* AfftectedItem = Cast<AExAffectedItem>(AffectedActor);
	if (!IsValid(AfftectedItem))
	{
		return;
	}

	if (EObjectType::Pawn == EffectData.ResultType)
	{

	}
	else if (EObjectType::WorldStatic == EffectData.ResultType)
	{

	}
	else if (EObjectType::Destructible == EffectData.ResultType)
	{
		ReqDestroyAffectItem(AfftectedItem);
	}

}

void AExPlayerController::ReqDestroyAffectItem_Implementation(AActor* AffectItem)
{
	UE_LOG(LogTemp, Warning, TEXT("aaa"))
	if (!AffectItem)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("bbb"))
	AffectItem->Destroy(true);
}

void AExPlayerController::MulticastAffectObjectResult_Implementation()
{

}
