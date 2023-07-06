#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IDamage.h"
#include "CRifleMissile.generated.h"

UCLASS()
class CPORTFOLIO_API ACRifleMissile 
	: public AActor
	, public IIDamage
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly)
		class UCapsuleComponent* Collision;

	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* ExplosionCollision;
	
	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleDefaultsOnly)
		class UNiagaraComponent* NiagaraComponent;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
		TSubclassOf<class ACExplosion> ExplosionClass;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
		TSubclassOf<class UMatineeCameraShake> ExplosionCameraShakeClass;

public:
	UPROPERTY(BlueprintReadWrite, Category = "HitData")
		FHitDatas HitData;

public:	
	ACRifleMissile();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void Tick(float DeltaTime) override;


public:
	void Shoot(class ACharacter* TargetActor);

	void LoadData(FHitDatas* InHitData);
	void GetOwner(class ACSkill1Rifle* InOwner);

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentBeginOverlap2(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);	// ExplosionOverlap

	UFUNCTION()
		void OnComponentEndOverlap2(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	class ACharacter* Target;
	class ACSkill1Rifle* Owner;

	UPROPERTY()
		TArray<class ACharacter*> OverlapCharacters;
};
