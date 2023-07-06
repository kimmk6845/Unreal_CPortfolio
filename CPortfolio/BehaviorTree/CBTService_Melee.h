#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Melee.generated.h"

UCLASS()
class CPORTFOLIO_API UCBTService_Melee : public UBTService
{
	GENERATED_BODY()
	
private:
	//���� ���� (�÷��̾���� �Ÿ��� �� ������ ������ ����)
	UPROPERTY(EditAnywhere, Category = "Action")
		float ActionRange = 150;

public:
	UCBTService_Melee();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory, float DeltaSeconds) override;

};
