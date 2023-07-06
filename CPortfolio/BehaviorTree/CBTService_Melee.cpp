#include "BehaviorTree/CBTService_Melee.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Components/CStateComponent.h"
#include "Components/CAIStateComponent.h"
#include "Components/CPatrolComponent.h"

/////////////////////////////// 미 구 현 //////////////////////////////

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";

	Interval = 0.1f;   //0.1초마다 서비스 체크 
	RandomDeviation = 0.0f;
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//BehaviorTree인 OwnerComp 
	//(Service 입장에서 자신을 소유한건 BehaviorTree라 이름이 이런듯)
	//그러면 BehaviorTree를 소유한애는 Controller임.
	//그래서 OwnerComp.GetOwner 는 비헤이비어 트리를 실행한 Controller임. 
	//이걸 우리가 만든 ACAIController형으로 캐스팅해서 controller에 저장.
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	//빙의된 대상은 controller의 GetPawn으로 가져옴. 
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	//ai의 State, AIState 컴포넌트 가져옴. 
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIStateComponent* aiState = CHelpers::GetComponent<UCAIStateComponent>(ai);

	//블프에서는 서비스에서 Hit처리를 하지않았지만 이번엔 여기서 해볼거임.
	//State가 Hitted 모드이면 aiState도 Hitted 모드로 변경시켜줌. 

	if (state->IsHittedMode())
	{
		aiState->SetHittedMode();
		//CLog::Print("Service: Hitted");

		// ####### TODO: 타겟을 잡아줌. 공격자를 aiState에 넣어줌
		// 히트당하면 블랙보드에 타겟을 세팅해줘야 함 ==> 컨트롤러에 함수
		// 공격자 액터를 불러오는 방법은? 
		// 무기 장착 중에 Hitted되면 무기가 손에 없음. equipping 중에hitted되면 begin_equip, end_equip 호출해줘야 함
		return;
	}

	//AIState로 부터 블랙보드의 PlayerKey 값을 가져와서 player에 넣어줌.
	ACPlayer* player = aiState->GetTargetPlayer();
	if (player == nullptr) //player가 없다면 즉, 인식한 대상이 없으면
	{
		UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(ai);
		if (!!(patrol->GetPatrol()) && state->IsIdleMode())		// idle일때만 패트롤하게 만들어 다른 동작중일 때, 움직이는 것 방지
		{
			aiState->SetPatrolMode();
			//CLog::Print("Service: Patrol");

			return;
		}

		aiState->SetWaitMode();
		//CLog::Print("Service: Wait");

		return;
	}

	//이 아래는 Player를 인식한 상태임.
	//ai와 player 사이의 거리 distance가
	float distance = ai->GetDistanceTo(player);

	//ActionRange보다 가까우면 Action
	if (distance < ActionRange)
	{
		aiState->SetActionMode();
		//CLog::Print("Service: Action");

		return;
	}

	//ActionRange보다 멀면 Approach
	aiState->SetApproachMode();
	//CLog::Print("Service: Approach");
}