#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CExplosion.generated.h"

UCLASS()
class CPORTFOLIO_API ACExplosion : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UNiagaraComponent* NiagaraComponent;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		class USoundWave* ExplosionSound;
	
public:	
	ACExplosion();

protected:
	virtual void BeginPlay() override;

};
