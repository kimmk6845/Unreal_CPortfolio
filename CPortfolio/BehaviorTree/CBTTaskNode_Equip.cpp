#include "BehaviorTree/CBTTaskNode_Equip.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Equip::UCBTTaskNode_Equip()
{
	NodeName = "Equip";

	bNotifyTick = true;  //�̰� �־�� tick�� ������. 
}

EBTNodeResult::Type UCBTTaskNode_Equip::ExecuteTask(UBehaviorTreeComponent&OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	//ai�� ���� Weapon Type�� Equip�� Type�� ������ ���� �� �ʿ� ������ �������� ������
	if (Type == weapon->GetWeaponType())
		return EBTNodeResult::Succeeded;

	//������ Ÿ�Կ� ���� ����ġ
	switch (Type)
	{
	case EWeaponType::OneHand : weapon->SetOneHandMode(); break;
	case EWeaponType::Assassin: weapon->SetAssassinMode(); break;
	case EWeaponType::Rifle: weapon->SetRifleMode(); break;
	}

	//���� ���λ��·� Execute�� �������ؾ��ϴϱ� InProgress ���·� ����
	return EBTNodeResult::InProgress;
}

//���� ������ �Ϸ�Ǹ� Idle ��尡 ��. Tick���� Idle ��������� üũ�ؼ�
//Idle ���� ���� �Ǿ����� ���� ���� �Ϸ�� �������� ���
void UCBTTaskNode_Equip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	if (state->IsIdleMode()) 
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}