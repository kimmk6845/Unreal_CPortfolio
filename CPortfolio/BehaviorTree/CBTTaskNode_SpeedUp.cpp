#include "BehaviorTree/CBTTaskNode_SpeedUp.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/CEnemy_AI.h"


UCBTTaskNode_SpeedUp::UCBTTaskNode_SpeedUp()
{
	NodeName = "Speed up";
}

EBTNodeResult::Type UCBTTaskNode_SpeedUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCharacterMovementComponent* moveComp = CHelpers::GetComponent<UCharacterMovementComponent>(ai);
	moveComp->MaxWalkSpeed = ImproveSpeed;

	//�׽�ũ�� ���� ���θ� �˷���� �������� �Ѿ.
	//���ǵ� �׽�ũ�� Sequence �ȿ��� �ӵ� ���� �����ϰ� �������� �Ѿ�� �ϴϱ�
	// ������ ����
	return EBTNodeResult::Succeeded;
}
