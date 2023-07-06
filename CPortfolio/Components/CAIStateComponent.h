#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAIStateComponent.generated.h"

//// @ AI 상태 타입 ENUM
///////////////////////////////////////////////////////////////////////////////////////////////////
UENUM(BlueprintType)
enum class EAIStateType : uint8
{
	Wait = 0, Approach, Action, Patrol, Hitted, Avoid, Dead, Max,
};
///////////////////////////////////////////////////////////////////////////////////////////////////


//// @ 상태 변환 델리게이트 선언
///////////////////////////////////////////////////////////////////////////////////////////////////
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIStateTypeChanged, EAIStateType, InPrevType, EAIStateType, InNewType);
///////////////////////////////////////////////////////////////////////////////////////////////////



UCLASS()	// 컴포넌트를 만들 때 meta의 BlueprintSpawnable을 빼줘야 에디터에서 임의로 할당 가능
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
	//// @ 현재 상태 확인 Getter
	////////////////////////////////////////////////////////////////////////////////
	FORCEINLINE bool IsWaitMode() { return GetType() == EAIStateType::Wait; }
	FORCEINLINE bool IsApproachMode() { return GetType() == EAIStateType::Approach; }
	FORCEINLINE bool IsActionMode() { return GetType() == EAIStateType::Action; }
	FORCEINLINE bool IsPatrolMode() { return GetType() == EAIStateType::Patrol; }
	FORCEINLINE bool IsHittedMode() { return GetType() == EAIStateType::Hitted; }
	FORCEINLINE bool IsAvoidMode() { return GetType() == EAIStateType::Avoid; }
	FORCEINLINE bool IsDeadMode() { return GetType() == EAIStateType::Dead; }
	////////////////////////////////////////////////////////////////////////////////


	//// @ 블랙보드 Setter
	////////////////////////////////////////////////////////////////////////////////
	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard)
	{
		//Controller에서 비헤이비어 트리를 가져오고 거기서 블랙보드를 가져온뒤에
		//여기 AIStateComponent에 넘겨서 Blackboard에 Set 해줌. 그때 사용할 함수 
		Blackboard = InBlackboard;
	}
	////////////////////////////////////////////////////////////////////////////////


	//// @ 현재 상태 적용 Setter
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
	FAIStateTypeChanged OnAIStateTypeChanged;	//<< 델리게이트


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
	//// @ 상태 타입 가져오는 내부함수
	////////////////////////////////////////////////////////////////////////////////
	EAIStateType GetType();
	////////////////////////////////////////////////////////////////////////////////


	//// @ 불랙보드의 키 값을 가져와 상태를 변환하는 함수
	////////////////////////////////////////////////////////////////////////////////
	void ChangeType(EAIStateType InType);
	////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////
///// <<<<< VARIABLE >>>>>> //////
//////////////////////////////////
private:
	UPROPERTY(EditAnywhere, Category = "Key")
		FName AIStateTypeKey = "AIStateType";		//<< Enum형식의 AI상태를 저장할 블랙보드 키

	UPROPERTY(EditAnywhere, Category = "Key")
		FName PlayerKey = "Player";					//<< 타겟 플레이어를 저장할 블랙보드 키

	UPROPERTY(EditAnywhere, Category = "Key")
		FName LocationKey = "Location";				//<< Patrol할 위치를 저장할 블랙보드 키

	class UBlackboardComponent* Blackboard;			//<< 블랙보드
		

/* =========================================== PRIVATE =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//

};
