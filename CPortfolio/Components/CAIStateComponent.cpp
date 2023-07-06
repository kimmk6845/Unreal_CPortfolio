#include "Components/CAIStateComponent.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ������
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
	//�����忡�� Object�� �������� �������µ� �������� PlayerKey �ΰͿ� ����
	//CPlayer�� ĳ�����ؼ� ����
	return Cast<ACPlayer>(Blackboard->GetValueAsObject(PlayerKey));
}

FVector UCAIStateComponent::GetLocation()
{
	//LocationKey�� ���� Vector �� ����
	return Blackboard->GetValueAsVector(LocationKey);
}

/* =========================================== PUBLIC =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* =========================================== PRIVATE =========================================== */


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ���� Ÿ�� �������� �����Լ�
//// TODO: �������� ���� Ű ���� ������
EAIStateType UCAIStateComponent::GetType()
{
	return (EAIStateType)Blackboard->GetValueAsEnum(AIStateTypeKey);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ �ҷ������� Ű ���� ������ ���¸� ��ȯ�ϴ� �Լ�
//// @ param 1: �ٲ� ���� ��
//// TODO: �����忡�� ���� ���� ���� �������� ���� ���·� �ٲ��� �� ��������Ʈ�� �̿��� ���� ���°� ����Ǿ��ٰ� �˸�
void UCAIStateComponent::ChangeType(EAIStateType InType)
{
	EAIStateType prevType = GetType();

	Blackboard->SetValueAsEnum(AIStateTypeKey, (uint8)InType);

	if (OnAIStateTypeChanged.IsBound())
		OnAIStateTypeChanged.Broadcast(prevType, InType);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
