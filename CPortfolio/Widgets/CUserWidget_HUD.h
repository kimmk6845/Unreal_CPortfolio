#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CWeaponComponent.h"
#include "CUserWidget_HUD.generated.h"

UCLASS()
class CPORTFOLIO_API UCUserWidget_HUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateMagazine(uint8 InCount, uint8 InMax);

	UFUNCTION(BlueprintImplementableEvent)
		void HiddenAmmo();
	
	UFUNCTION(BlueprintImplementableEvent)
		void VisibleAmmo();

};
