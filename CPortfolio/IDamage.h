#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Engine/DataTable.h"	// 데이터 테이블

#include "IDamage.generated.h"

USTRUCT(BlueprintType)
struct FDoActionDatas : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* DoActionMontage = nullptr;

	UPROPERTY(EditAnywhere)
		float DoAction_MontagePlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		bool bFixedCamera = false;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;
};


USTRUCT(BlueprintType)
struct FHitDatas : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float Damage = 0.0f;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* HitMontage = nullptr;

	UPROPERTY(EditAnywhere)
		float Hit_MontagePlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	UPROPERTY(EditAnywhere)
		class USoundWave* Sound = nullptr;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UMatineeCameraShake> ShakeClass = nullptr;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect = nullptr;

	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* NGEffect = nullptr;

	UPROPERTY(EditAnywhere)
		FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
		float HitStop = 0.0f;

	UPROPERTY(EditAnywhere)
		float Launch = 0.0f;

	UPROPERTY(EditAnywhere)
		bool bForceAnimation = false;

};

USTRUCT(BlueprintType)
struct FAirComboDatas : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AirComboMontage = nullptr;

	UPROPERTY(EditAnywhere)
		float AirCombo_MontagePlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		bool bFixedCamera = false;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	UPROPERTY(EditAnywhere)
		float Launch = 0.0f;
};

UINTERFACE(MinimalAPI)
class UIDamage : public UInterface
{
	GENERATED_BODY()
};

class CPORTFOLIO_API IIDamage
{
	GENERATED_BODY()

};
