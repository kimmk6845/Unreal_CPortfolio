#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Target.generated.h"

UCLASS()
class CPORTFOLIO_API UCUserWidget_Target : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void VisibleTarget();

	UFUNCTION(BlueprintImplementableEvent)
		void HiddenTarget();

};
