#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSkill1OneHand.generated.h"

UCLASS()
class CPORTFOLIO_API ACSkill1OneHand : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class UCapsuleComponent* Collision;

	UPROPERTY(VisibleDefaultsOnly)
		class UNiagaraComponent* NiagaraComponent;

public:	
	ACSkill1OneHand();

protected:
	virtual void BeginPlay() override;

public:
	void Shoot(FVector InDirection, FRotator InRotation);

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	UPROPERTY()
		TArray<class ACharacter*> Hitted;
};
