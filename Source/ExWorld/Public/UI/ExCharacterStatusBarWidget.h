
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExCharacterStatusBarWidget.generated.h"


class UProgressBar;



/**
 * 
 */
UCLASS()
class EXWORLD_API UExCharacterStatusBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetHealthPercentage(int32 CurrentHP);


public:
	UPROPERTY(Meta = (BindWidget))
	UProgressBar* ProgressBar_Health = nullptr;

};
