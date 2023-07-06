#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPatrolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPORTFOLIO_API UCPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Path")
		class ACPatrolActor* Patrol;

	UPROPERTY(EditAnywhere, Category = "Path")
		int32 Index;

	UPROPERTY(EditAnywhere, Category = "Path")
		bool bReverse;

	UPROPERTY(EditAnywhere, Category = "Path")
		float AcceptanceDistance = 10;

public:
	FORCEINLINE bool IsValidPatrol() { return Patrol != nullptr; }

public:
	UCPatrolComponent();

public:
	bool GetMoveTo(FVector& OutLocation, float& OutAcceptanceDistance);
	void Update();
	class ACPatrolActor* GetPatrol() { return Patrol; }
};
