#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAIStateComponent.generated.h"

//// @ AI ���� Ÿ�� ENUM
///////////////////////////////////////////////////////////////////////////////////////////////////
UENUM(BlueprintType)
enum class EAIStateType : uint8
{
	Wait = 0, Approach, Action, Patrol, Hitted, Avoid, Dead, Max,
};
///////////////////////////////////////////////////////////////////////////////////////////////////


//// @ ���� ��ȯ ��������Ʈ ����
///////////////////////////////////////////////////////////////////////////////////////////////////
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIStateTypeChanged, EAIStateType, InPrevType, EAIStateType, InNewType);
///////////////////////////////////////////////////////////////////////////////////////////////////



UCLASS()	// ������Ʈ�� ���� �� meta�� BlueprintSpawnable�� ����� �����Ϳ��� ���Ƿ� �Ҵ� ����
class CPORTFOLIO_API UCAIStateComponent : public UActorComponent
{
	GENERATED_BODY()

////  @ Default Class Function
///////////////////////////////////////////////////////////////////////////////////////////////////
public:	
	UCAIStateComponent();

protected:
	virtual void BeginPlay() override;
///////////////////////////////////////////////////////////////////////////////////////////////////


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* =========================================== PUBLIC =========================================== */

//////////////////////////////////
///// <<<<< FUNCTION >>>>>> //////
//////////////////////////////////
public:
	//// @ ���� ���� Ȯ�� Getter
	////////////////////////////////////////////////////////////////////////////////
	FORCEINLINE bool IsWaitMode() { return GetType() == EAIStateType::Wait; }
	FORCEINLINE bool IsApproachMode() { return GetType() == EAIStateType::Approach; }
	FORCEINLINE bool IsActionMode() { return GetType() == EAIStateType::Action; }
	FORCEINLINE bool IsPatrolMode() { return GetType() == EAIStateType::Patrol; }
	FORCEINLINE bool IsHittedMode() { return GetType() == EAIStateType::Hitted; }
	FORCEINLINE bool IsAvoidMode() { return GetType() == EAIStateType::Avoid; }
	FORCEINLINE bool IsDeadMode() { return GetType() == EAIStateType::Dead; }
	////////////////////////////////////////////////////////////////////////////////


	//// @ ������ Setter
	////////////////////////////////////////////////////////////////////////////////
	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard)
	{
		//Controller���� �����̺�� Ʈ���� �������� �ű⼭ �����带 �����µڿ�
		//���� AIStateComponent�� �Ѱܼ� Blackboard�� Set ����. �׶� ����� �Լ� 
		Blackboard = InBlackboard;
	}
	////////////////////////////////////////////////////////////////////////////////


	//// @ ���� ���� ���� Setter
	////////////////////////////////////////////////////////////////////////////////
	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetPatrolMode();
	void SetHittedMode();
	void SetAvoidMode();
	void SetDeadMode();
	////////////////////////////////////////////////////////////////////////////////


public:
	class ACPlayer* GetTargetPlayer();
	FVector GetLocation();

//////////////////////////////////
///// <<<<< VARIABLE >>>>>> //////
//////////////////////////////////
public:
	FAIStateTypeChanged OnAIStateTypeChanged;	//<< ��������Ʈ


/* =========================================== PUBLIC =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* =========================================== PRIVATE =========================================== */

//////////////////////////////////
///// <<<<< FUNCTION >>>>>> //////
//////////////////////////////////
private:
	//// @ ���� Ÿ�� �������� �����Լ�
	////////////////////////////////////////////////////////////////////////////////
	EAIStateType GetType();
	////////////////////////////////////////////////////////////////////////////////


	//// @ �ҷ������� Ű ���� ������ ���¸� ��ȯ�ϴ� �Լ�
	////////////////////////////////////////////////////////////////////////////////
	void ChangeType(EAIStateType InType);
	////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////
///// <<<<< VARIABLE >>>>>> //////
//////////////////////////////////
private:
	UPROPERTY(EditAnywhere, Category = "Key")
		FName AIStateTypeKey = "AIStateType";		//<< Enum������ AI���¸� ������ ������ Ű

	UPROPERTY(EditAnywhere, Category = "Key")
		FName PlayerKey = "Player";					//<< Ÿ�� �÷��̾ ������ ������ Ű

	UPROPERTY(EditAnywhere, Category = "Key")
		FName LocationKey = "Location";				//<< Patrol�� ��ġ�� ������ ������ Ű

	class UBlackboardComponent* Blackboard;			//<< ������
		

/* =========================================== PRIVATE =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//

};
