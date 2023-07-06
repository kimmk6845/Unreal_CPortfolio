#include "Components/CAIStateComponent.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 생성자
UCAIStateComponent::UCAIStateComponent()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Begin Play
void UCAIStateComponent::BeginPlay()
{
	Super::BeginPlay();
}
///////////////////////////////////////////////////////////////////////////////////////////////////



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* =========================================== PUBLIC =========================================== */


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Setter
void UCAIStateComponent::SetWaitMode()
{
	ChangeType(EAIStateType::Wait);
}
void UCAIStateComponent::SetApproachMode()
{
	ChangeType(EAIStateType::Approach);
}
void UCAIStateComponent::SetActionMode()
{
	ChangeType(EAIStateType::Action);
}
void UCAIStateComponent::SetPatrolMode()
{
	ChangeType(EAIStateType::Patrol);
}
void UCAIStateComponent::SetHittedMode()
{
	ChangeType(EAIStateType::Hitted);
}
void UCAIStateComponent::SetAvoidMode()
{
	ChangeType(EAIStateType::Avoid);
}
void UCAIStateComponent::SetDeadMode()
{
	ChangeType(EAIStateType::Dead);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


ACPlayer* UCAIStateComponent::GetTargetPlayer()
{
	//블랙보드에서 Object형 변수값을 가져오는데 변수명이 PlayerKey 인것에 대해
	//CPlayer로 캐스팅해서 리턴
	return Cast<ACPlayer>(Blackboard->GetValueAsObject(PlayerKey));
}

FVector UCAIStateComponent::GetLocation()
{
	//LocationKey에 대한 Vector 값 리턴
	return Blackboard->GetValueAsVector(LocationKey);
}

/* =========================================== PUBLIC =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* =========================================== PRIVATE =========================================== */


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 상태 타입 가져오는 내부함수
//// TODO: 블랙보드의 상태 키 값을 가져옴
EAIStateType UCAIStateComponent::GetType()
{
	return (EAIStateType)Blackboard->GetValueAsEnum(AIStateTypeKey);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 불랙보드의 키 값을 가져와 상태를 변환하는 함수
//// @ param 1: 바뀔 상태 값
//// TODO: 블랙보드에서 현재 상태 값을 가져오고 현재 상태로 바꿔준 후 델리게이트를 이용해 현재 상태가 변경되었다고 알림
void UCAIStateComponent::ChangeType(EAIStateType InType)
{
	EAIStateType prevType = GetType();

	Blackboard->SetValueAsEnum(AIStateTypeKey, (uint8)InType);

	if (OnAIStateTypeChanged.IsBound())
		OnAIStateTypeChanged.Broadcast(prevType, InType);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
