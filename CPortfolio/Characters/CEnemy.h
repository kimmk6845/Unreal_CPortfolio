#pragma once

#include "CoreMinimal.h"
#include "Characters/CCharacter.h"

#include "CEnemy.generated.h"

UCLASS()
class CPORTFOLIO_API ACEnemy : public ACCharacter
{
	GENERATED_BODY()

public:
	ACEnemy();
	
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaTime) override;
};
