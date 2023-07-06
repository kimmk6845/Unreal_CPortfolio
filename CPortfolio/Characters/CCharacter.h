#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "IDamage.h"
#include "CCharacter.generated.h"

USTRUCT()
struct FHitDamageEvent : public FDamageEvent
{
	GENERATED_BODY()

public:
	FHitDatas* HitData;
};


UCLASS()
class CPORTFOLIO_API ACCharacter 
	: public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		float ViewInterpSpeed = 25;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		FVector2D PitchRange = FVector2D(-75, +40);

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		FVector2D YawRange = FVector2D(-90, +90);

public:
	// Weapon, State, Status는 상속
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		class UCWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		class UCStateComponent* StateComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		class UCStatusComponent* StatusComponent;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CharacterState")
		float Health;

	UPROPERTY(EditAnywhere, Category = "CharacterState")
		float MaxHealth;

public:
	FORCEINLINE int JumpCount() { return jumpCount; }
	FORCEINLINE float GetAimPitch() { return AimPitch; }
	FORCEINLINE float GetAimYaw() { return AimYaw; }

	FORCEINLINE float GetHealth() { return Health; }
public:
	ACCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Damaged(class ACharacter* InAttacker, class AActor* InCauser, struct FHitDatas* InHitData);

public:
	virtual void PlayAnimation(class ACharacter* InOwner);
	virtual void ForcePlayAnimation(class ACharacter* InOwner);		// 어떤 몽타주가 실행되고 있더라도 무시하고 실행
	virtual void PlayHitStop(class UWorld* InWorld);
	virtual void PlaySound(class ACharacter* InOwner);
	virtual void PlayEffect(class UWorld* InWorld, const FVector& InLocation);
	virtual void PlayCameraShake();

protected:
	// Action
	virtual void DoAction();
	virtual void End_Fire();
	virtual void OneHand();
	virtual void Assassin();
	virtual void Rifle();

protected:
	virtual void Reload();
	virtual void Begin_SubAction();
	virtual void End_SubAction();

protected:
	virtual void SkillQ();
	virtual void SkillE();
	virtual void SubSkill();

protected:
	virtual void Avoid();

protected:
	float CurrSpeed;			// 속도 보간 변수

protected:
	int jumpCount;

protected:
	float AimYaw;
	float AimPitch;

protected:
	struct FDamageData
	{
		class ACharacter* Attacker;
		class AActor* Causer;
		struct FHitDatas* HitData;
	} Damage;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

private:
	void Hitted();
	void Dead();
};
