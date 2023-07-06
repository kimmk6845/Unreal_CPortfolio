#include "Components/CStateComponent.h"
#include "Global.h"

UCStateComponent::UCStateComponent()
{
}


void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCStateComponent::ChangeType(EStateType inType)
{
	EStateType PrevType = Type;
	Type = inType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(PrevType, inType);
}


// Set
void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetAvoidMode()
{
	ChangeType(EStateType::Avoid);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}

void UCStateComponent::SetParkourMode()
{
	ChangeType(EStateType::Parkour);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetJumpMode()
{
	ChangeType(EStateType::Jump);
}

void UCStateComponent::SetReloadMode()
{
	ChangeType(EStateType::Reload);
}

void UCStateComponent::SetSkillQMode()
{
	ChangeType(EStateType::SkillQ);
}

void UCStateComponent::SetSkillEMode()
{
	ChangeType(EStateType::SkillE);
}

void UCStateComponent::SetSubSkillMode()
{
	ChangeType(EStateType::SubSkill);
}