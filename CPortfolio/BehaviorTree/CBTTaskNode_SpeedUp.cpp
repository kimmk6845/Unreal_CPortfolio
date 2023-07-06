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

	//테스크의 성공 여부를 알려줘야 다음으로 넘어감.
	//스피드 테스크는 Sequence 안에서 속도 설정 성공하고 다음으로 넘어가야 하니까
	// 성공을 리턴
	return EBTNodeResult::Succeeded;
}
