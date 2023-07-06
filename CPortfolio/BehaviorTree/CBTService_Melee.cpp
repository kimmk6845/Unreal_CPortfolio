#include "BehaviorTree/CBTService_Melee.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Components/CStateComponent.h"
#include "Components/CAIStateComponent.h"
#include "Components/CPatrolComponent.h"

/////////////////////////////// �� �� �� //////////////////////////////

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";

	Interval = 0.1f;   //0.1�ʸ��� ���� üũ 
	RandomDeviation = 0.0f;
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//BehaviorTree�� OwnerComp 
	//(Service ���忡�� �ڽ��� �����Ѱ� BehaviorTree�� �̸��� �̷���)
	//�׷��� BehaviorTree�� �����Ѿִ� Controller��.
	//�׷��� OwnerComp.GetOwner �� �����̺�� Ʈ���� ������ Controller��. 
	//�̰� �츮�� ���� ACAIController������ ĳ�����ؼ� controller�� ����.
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	//���ǵ� ����� controller�� GetPawn���� ������. 
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	//ai�� State, AIState ������Ʈ ������. 
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIStateComponent* aiState = CHelpers::GetComponent<UCAIStateComponent>(ai);

	//���������� ���񽺿��� Hitó���� �����ʾ����� �̹��� ���⼭ �غ�����.
	//State�� Hitted ����̸� aiState�� Hitted ���� ���������. 

	if (state->IsHittedMode())
	{
		aiState->SetHittedMode();
		//CLog::Print("Service: Hitted");

		// ####### TODO: Ÿ���� �����. �����ڸ� aiState�� �־���
		// ��Ʈ���ϸ� �����忡 Ÿ���� ��������� �� ==> ��Ʈ�ѷ��� �Լ�
		// ������ ���͸� �ҷ����� �����? 
		// ���� ���� �߿� Hitted�Ǹ� ���Ⱑ �տ� ����. equipping �߿�hitted�Ǹ� begin_equip, end_equip ȣ������� ��
		return;
	}

	//AIState�� ���� �������� PlayerKey ���� �����ͼ� player�� �־���.
	ACPlayer* player = aiState->GetTargetPlayer();
	if (player == nullptr) //player�� ���ٸ� ��, �ν��� ����� ������
	{
		UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(ai);
		if (!!(patrol->GetPatrol()) && state->IsIdleMode())		// idle�϶��� ��Ʈ���ϰ� ����� �ٸ� �������� ��, �����̴� �� ����
		{
			aiState->SetPatrolMode();
			//CLog::Print("Service: Patrol");

			return;
		}

		aiState->SetWaitMode();
		//CLog::Print("Service: Wait");

		return;
	}

	//�� �Ʒ��� Player�� �ν��� ������.
	//ai�� player ������ �Ÿ� distance��
	float distance = ai->GetDistanceTo(player);

	//ActionRange���� ������ Action
	if (distance < ActionRange)
	{
		aiState->SetActionMode();
		//CLog::Print("Service: Action");

		return;
	}

	//ActionRange���� �ָ� Approach
	aiState->SetApproachMode();
	//CLog::Print("Service: Approach");
}