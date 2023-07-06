#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IDamage.h"
#include "CSkill1Rifle.generated.h"

UCLASS()
class CPORTFOLIO_API ACSkill1Rifle 
	: public AActor
	, public IIDamage
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Collision;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* MissileLeft;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* MissileRight;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
		uint8 MaxAmmo = 50;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
		float ReloadTime = 5.0f;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Fire")
		FName FlashBoneName;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		class USoundWave* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		class UParticleSystem* FlashParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		float AutoFireInterval = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		TSubclassOf<class ACBullet> BulletClass;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		TSubclassOf<class UMatineeCameraShake> CameraShakeClass;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Missile")
		class USoundWave* MissileSound;

	UPROPERTY(VisibleDefaultsOnly, Category = "Missile")
		FName MissileBoneName[2] = { "Missile_Right", "Missile_Left" };

	UPROPERTY(EditDefaultsOnly, Category = "Missile")
		TSubclassOf<class ACRifleMissile> MissileClass;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Hit")
		class UMaterialInstanceConstant* HitDecal;

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
		class UParticleSystem* HitParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
		class UNiagaraSystem* HitNiagara;

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
		float HitDistance = 3000.0f;

//// Skill Data/////////////////////////////
public:
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		TArray<FHitDatas> SkillQHitDatas;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "DataTable")
		class UDataTable* SkillQHitDataTable;

////////////////////////////////////////////


public:	
	ACSkill1Rifle();

protected:
	virtual void BeginPlay() override;

protected:	
	virtual void Tick(float DeltaTime) override;

public:
	void ExplosionDamage(TArray<class ACharacter*> InHitCharacters);


private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	void Begin_Fire();
	void End_Fire();

	void OnFiring();

private:
	void Reload();

private:
	void Explosion();

private:
	UPROPERTY()
		TArray<class ACharacter*> OverlapActors;

	class ACharacter* Candidate;
	float MaxRadius = 1000.0f;

private:
	FTimerHandle AutoFireHandle;

private:
	uint8 CurrentAmmo;
	float CurrRecoil;

	float MaterialScalarValue = -0.5f;

private:
	bool bReloading = false;
	
};
