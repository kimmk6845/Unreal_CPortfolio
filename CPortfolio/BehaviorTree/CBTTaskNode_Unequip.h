#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Unequip.generated.h"

UCLASS()
class CPORTFOLIO_API UCBTTaskNode_Unequip : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_Unequip();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent&
		OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory, float DeltaSeconds) override;
};
