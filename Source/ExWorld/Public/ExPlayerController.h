// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ExAffectedItem.h"
#include "ExPlayerController.generated.h"



class UExTimeLimitationShow;


/**
 * 
 */
UCLASS()
class EXWORLD_API AExPlayerController : public APlayerController
{
	GENERATED_BODY()
	



public:

	virtual void BeginPlay() override;

	void ShowTimeLimitation(const int64 TimeLeft);

	void OnApplyEffect(AActor* AffectedActor, const FEffectData& EffectData);
	
	bool GetEffectDataByObjectType(const EObjectType ObjectType, FEffectData& EffectData);

	UFUNCTION(Server, reliable, BlueprintCallable)
	void ReqDestroyAffectItem(AActor* AffectItem);
	UFUNCTION(NetMulticast, reliable)
	void MulticastAffectObjectResult();

	UPROPERTY(EditAnywhere, Category = "ExWorldTest|UI")
	TSubclassOf<UExTimeLimitationShow> TimeLimitationClass;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "ExWorld|UI")
	UExTimeLimitationShow* UITimeLimitation;

private:
	TMap<EObjectType, FEffectData> ConfigureData;


};
