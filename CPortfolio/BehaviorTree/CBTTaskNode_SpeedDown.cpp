#include "BehaviorTree/CBTTaskNode_SpeedDown.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/CEnemy_AI.h"


UCBTTaskNode_SpeedDown::UCBTTaskNode_SpeedDown()
{
	NodeName = "Speed down";
}

EBTNodeResult::Type UCBTTaskNode_SpeedDown::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCharacterMovementComponent* moveComp = CHelpers::GetComponent<UCharacterMovementComponent>(ai);
	moveComp->MaxWalkSpeed = DownSpeed;

	return EBTNodeResult::Succeeded;
}
