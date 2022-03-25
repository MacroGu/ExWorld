// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "ExAffectedItem.generated.h"




class USphereComponent;

UENUM(BlueprintType)
enum class EObjectType : uint8
{
	None = 0,
	Pawn,
	WorldStatic,
	Destructible
};


USTRUCT(BlueprintType)
struct EXWORLD_API FEffectData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	FString Result;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	EObjectType ResultType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	int32 HpChange;

};


USTRUCT(BlueprintType, meta = (DisplayName = "Configure Data Affect"))
struct EXWORLD_API FAffectDataTable : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	EObjectType AffectType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FEffectData AffectData;

};


UCLASS()
class EXWORLD_API AExAffectedItem : public AActor
{
	GENERATED_BODY()


	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

public:	
	// Sets default values for this actor's properties
	AExAffectedItem();

	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EObjectType ObjectType;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
