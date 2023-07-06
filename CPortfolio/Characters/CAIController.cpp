#include "Characters/CAIController.h"
#include "Global.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Components/CAIStateComponent.h"

ACAIController::ACAIController()
{
	//Perception ������Ʈ ����
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");
	//�����常�� �θ��� Blackboard ������ �־���. 
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");

	//Sight�� ������Ʈ�� �ƴ����� �����ڿ��� �����Ҵ��� �Ұ���.
	//�����Ҵ� ����� 2������ ����.
	//�����ڿ��� : CreateDefaultSubobject �� �Ҵ�
	//��Ÿ�ӿ��� : NewObject �� �Ҵ� (������ �÷��Ϳ� ���� ������ ��.)
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 1000;  //�þ� ���� �ݰ�
	Sight->LoseSightRadius = 1500;   //�þ� ����� 
	Sight->PeripheralVisionAngleDegrees = 60;  //�þ߰� 
	Sight->SetMaxAge(2);

	Sight->DetectionByAffiliation.bDetectEnemies = true;   //�� �� ���� �ϰڴ�.
	Sight->DetectionByAffiliation.bDetectNeutrals = false;  //�߸��� �������� �ʰڴ�. 
	Sight->DetectionByAffiliation.bDetectFriendlies = false;  //�Ʊ��� �������� �ʰڴ�. 

	Perception->ConfigureSense(*Sight);   //SenseConfig ����
	//�������� �ִٸ� �迭�� ������� �ϴ°���. �츮�� Sight �ϳ��� ������

	Perception->SetDominantSense(*Sight->GetSenseImplementation());
	//������ ������ �켱������ ���ϴ°���. 
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	//���������� OnPerceptionUpdated ��������Ʈ ȣ�� 
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
}

//���� �ɶ� ȣ��� �Լ� 
void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Enemy = Cast<ACEnemy_AI>(InPawn); //���ǵ� ��Ʈ�ѷ� ������(�� ĳ����) ����

	//TeamID�� Controller���� ������.
	SetGenericTeamId(Enemy->GetTeamID());

	//������ ����.
	UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset, Blackboard);

	//Enemy�� ���� AIStateComponent ������.
	AIState = CHelpers::GetComponent<UCAIStateComponent>(Enemy);
	//AIStateComponent�� Blackboard �Ѱ���.
	AIState->SetBlackboard(Blackboard);

	//�����̺�� Ʈ�� ����.
	RunBehaviorTree(Enemy->GetBehaviorTree());

}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();

}

//���������� 
void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	//������ ����Ʈ�� �켱 actors�� �޾ƿ�.
	TArray<AActor*> actors;
	//GetCurrentlyPerceivedActors�� ���� ������ ���͵��� ��������. 
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);	//TeamID �����ؼ� ������

	//actors�� player�� �ִ����� üũ (������ �ο��� �ϸ� ACPlayer�� �ƴ� Character �� �޾ƾ���)
	ACPlayer* player = nullptr;
	for (AActor* actor : actors)
	{
		player = Cast<ACPlayer>(actor);
		CLog::Print(player);
		if (!!player)
			break;
	}

	//�������� Player ������ ������ player�� �־���. 
	Blackboard->SetValueAsObject("Player", player);
}