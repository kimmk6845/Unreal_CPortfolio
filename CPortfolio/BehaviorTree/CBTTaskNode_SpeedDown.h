#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_SpeedDown.generated.h"

UCLASS()
class CPORTFOLIO_API UCBTTaskNode_SpeedDown : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "value")
		float DownSpeed = 300.0f;	// 스피드 다운 값

public:
	UCBTTaskNode_SpeedDown();

private:
	//UBTTaskNode 부모에 있는 ExecuteTask 함수는 private으로 선언되었지만 
	//자식에서 재정의 가능함. C에서는 불가능한 문법이지만, 언리얼 C에서는 가능함.
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;
};
