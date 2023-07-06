#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/CWeaponComponent.h"
#include "CBTTaskNode_Equip.generated.h"

UCLASS()
class CPORTFOLIO_API UCBTTaskNode_Equip : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Type")
		EWeaponType Type;	//<< ������ ���� Ÿ��(�������� ����)

public:
	UCBTTaskNode_Equip();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent&
		OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory, float DeltaSeconds) override;
};
