#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSkill2OneHand.generated.h"

UCLASS()
class CPORTFOLIO_API ACSkill2OneHand : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Collision;

public:	
	ACSkill2OneHand();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UPROPERTY()
		TArray<class ACharacter*> Targets;	// 당겨지는 케릭터 배열

};
