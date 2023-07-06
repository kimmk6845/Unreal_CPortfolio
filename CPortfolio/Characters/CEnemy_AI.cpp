#include "Characters/CEnemy_AI.h"
#include "Global.h"
#include "Components/CAIStateComponent.h"
#include "Components/CPatrolComponent.h"

ACEnemy_AI::ACEnemy_AI()
{
	CHelpers::CreateActorComponent<UCAIStateComponent>(this, &AIState, "AIState");
	CHelpers::CreateActorComponent<UCPatrolComponent>(this, &Patrol, "CPatrol");

}

void ACEnemy_AI::BeginPlay()
{
	Super::BeginPlay();



}
