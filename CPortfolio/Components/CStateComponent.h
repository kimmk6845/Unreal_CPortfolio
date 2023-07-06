#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle, Avoid, Equip, Action, Hitted, Parkour, Dead, Jump, Reload, SkillQ, SkillE, SubSkill
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPORTFOLIO_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
	FORCEINLINE bool IsAvoidMode() { return Type == EStateType::Avoid; }
	FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }
	FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
	FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }
	FORCEINLINE bool IsParkourMode() { return Type == EStateType::Parkour; }
	FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }
	FORCEINLINE bool IsJumpMode() { return Type == EStateType::Jump; }
	FORCEINLINE bool IsReloadMode() { return Type == EStateType::Reload; }
	FORCEINLINE bool IsSkillQMode() { return Type == EStateType::SkillQ; }
	FORCEINLINE bool IsSkillEMode() { return Type == EStateType::SkillE; }
	FORCEINLINE bool IsSubSkillMode() { return Type == EStateType::SubSkill; }


public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

private:
	void ChangeType(EStateType inType);

public:
	void SetIdleMode();
	void SetAvoidMode();
	void SetEquipMode();
	void SetActionMode();
	void SetHittedMode();
	void SetParkourMode();
	void SetDeadMode();
	void SetJumpMode();
	void SetReloadMode();
	void SetSkillQMode();
	void SetSkillEMode();
	void SetSubSkillMode();


public:
	// 델리게이트
	FStateTypeChanged OnStateTypeChanged;

// 변수
private:
	EStateType Type = EStateType::Idle;
};
