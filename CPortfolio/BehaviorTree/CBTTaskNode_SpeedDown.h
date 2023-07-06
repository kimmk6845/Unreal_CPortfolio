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
		float DownSpeed = 300.0f;	// ���ǵ� �ٿ� ��

public:
	UCBTTaskNode_SpeedDown();

private:
	//UBTTaskNode �θ� �ִ� ExecuteTask �Լ��� private���� ����Ǿ����� 
	//�ڽĿ��� ������ ������. C������ �Ұ����� ����������, �𸮾� C������ ������.
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;
};
