#include "BehaviorTree/CBTTaskNode_Patrol.h"
#include "Global.h"
#include "Components/CPatrolComponent.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"

UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(ai);
	CheckNullResult(patrol, EBTNodeResult::Failed);


	FVector location;
	float distance;

	if (patrol->GetMoveTo(location, distance) == false)
		return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(ai);

	FVector location;
	float distance;

	patrol->GetMoveTo(location, distance);
	EPathFollowingRequestResult::Type type = controller->MoveToLocation(location, distance, false);

	if (type == EPathFollowingRequestResult::Failed)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		return;
	}

	if (type == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		patrol->Update();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		return;
	}
}