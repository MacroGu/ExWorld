#include "ExCharacterStatusBarWidget.h"

#include "Components/ProgressBar.h"




void UExCharacterStatusBarWidget::SetHealthPercentage(int32 CurrentHP)
{
	float fHP = CurrentHP;

	ProgressBar_Health->SetPercent(fHP / 100.f);
}


