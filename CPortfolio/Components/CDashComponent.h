#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CDashComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPORTFOLIO_API UCDashComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCDashComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UAnimMontage* GetMontage();
	float GetDistance();

private:
	UPROPERTY(EditDefaultsOnly)
		class UAnimMontage* DashMontage;

	UPROPERTY(EditDefaultsOnly)
		float DashDistance;

	class ACharacter* Owner;		//<< 컴포넌트 소유 케릭터

};
