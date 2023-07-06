#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_SpeedUp.generated.h"

UCLASS()
class CPORTFOLIO_API UCBTTaskNode_SpeedUp : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "value")
		float ImproveSpeed = 600.0f;	// ���ǵ� ��� ��

public:
	UCBTTaskNode_SpeedUp();

private:
	//UBTTaskNode �θ� �ִ� ExecuteTask �Լ��� private���� ����Ǿ����� 
	//�ڽĿ��� ������ ������. C������ �Ұ����� ����������, �𸮾� C������ ������.
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;
};
