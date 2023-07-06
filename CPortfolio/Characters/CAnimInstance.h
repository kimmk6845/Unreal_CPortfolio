#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CWeaponComponent.h"
#include "Components/CFeetComponent.h"
#include "CAnimInstance.generated.h"

UCLASS()
class CPORTFOLIO_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float Direction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float AimPitch;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
		bool Falling;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
		int JumpCount;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		EWeaponType WeaponType = EWeaponType::Unarmed;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Guns")
		bool bInAim;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Guns")
		bool bFiring;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Guns")
		FVector LeftHandLocation = FVector(-27, 10.5f, 5);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Guns")
		bool bUseIK;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
		bool bFeet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
		FFeetData FeetData;

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	void SetPlayerState();
	void SetWeaponState();

private:
	UFUNCTION()
		void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

private:
	class ACCharacter* OwnerCharacter;
	class UCWeaponComponent* Weapon;

};


