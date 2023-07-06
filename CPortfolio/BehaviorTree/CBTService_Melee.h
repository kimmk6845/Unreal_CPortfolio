#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Melee.generated.h"

UCLASS()
class CPORTFOLIO_API UCBTService_Melee : public UBTService
{
	GENERATED_BODY()
	
private:
	//공격 범위 (플레이어와의 거리가 이 안으로 들어오면 공격)
	UPROPERTY(EditAnywhere, Category = "Action")
		float ActionRange = 150;

public:
	UCBTService_Melee();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory, float DeltaSeconds) override;

};
