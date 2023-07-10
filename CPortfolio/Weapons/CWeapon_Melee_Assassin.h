#pragma once

#include "CoreMinimal.h"
#include "Weapons/CWeapon_Melee.h"
#include "CWeapon_Melee_Assassin.generated.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ CONST GLOBAL VARIABLE
const FVector DisableTeleportingLocation = FVector::ZeroVector + FVector(0, 0, 1e9); //<< �ڷ���Ʈ �Ұ� ����
///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class CPORTFOLIO_API ACWeapon_Melee_Assassin : public ACWeapon_Melee
{
	GENERATED_BODY()

////  @ Default Class Function
///////////////////////////////////////////////////////////////////////////////////////////////////
public:
	ACWeapon_Melee_Assassin();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
///////////////////////////////////////////////////////////////////////////////////////////////////


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* =========================================== PRIVATE =========================================== */

//////////////////////////////////
///// <<<<< FUNCTION >>>>>> //////
//////////////////////////////////
private:
	//// @ ���� �� ���� ��Ƽ���� �Լ�
	////////////////////////////////////////////////////////////////////////////////
	void Begin_Equip() override;
	void End_UnEquip() override;
	////////////////////////////////////////////////////////////////////////////////
	
	
	//// @ Q ��ų ��Ƽ���� �Լ�
	////////////////////////////////////////////////////////////////////////////////
	void SkillQ() override;
	void Begin_SkillQ() override;
	void End_SkillQ() override;
	////////////////////////////////////////////////////////////////////////////////


	//// @ E ��ų ��Ƽ���� �Լ�
	////////////////////////////////////////////////////////////////////////////////
	void SkillE() override;
	void Begin_SkillE() override;
	void End_SkillE() override;
	////////////////////////////////////////////////////////////////////////////////


	//// @ ���� �޺� �����
	////////////////////////////////////////////////////////////////////////////////
	void BackAttack() override;
	////////////////////////////////////////////////////////////////////////////////


	//// @ SubSkill (Input X Key)
	////////////////////////////////////////////////////////////////////////////////
	void SubSkill() override;
	////////////////////////////////////////////////////////////////////////////////


	//// @ Q ��ų �ڷ���Ʈ ��ġ Ž�� �Լ�
	////////////////////////////////////////////////////////////////////////////////
	FVector TraceCanTeleportLocation(class ACharacter* target);
	bool TraceBeingFloor(FVector capsuleLocation);
	////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////
///// <<<<< VARIABLE >>>>>> //////
//////////////////////////////////
private:
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		class UStaticMeshComponent* Sword;				//<< Į �޽�

	UPROPERTY(EditDefaultsOnly, Category = "Skill Q")
		class UAnimMontage* SkillQAttackMontages[5];	//<< Q��ų ���� �ִϸ��̼� ��Ÿ�� �迭

	UPROPERTY(EditDefaultsOnly, Category = "Skill Q")
		class UAnimMontage* SkillQHitMontages[5];		//<< Q��ų �ǰ� �ִϸ��̼� ��Ÿ�� �迭

	UPROPERTY(EditDefaultsOnly, Category = "Skill Q")
		float distanceToEnemy[5] = { 180.0f, 200.0f, 134.0f, 110.0f, 160.0f };	//<< Q��ų ������ �Ÿ�

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
		TSubclassOf<class UCUserWidget_Target> TargetWidgetClass;	//<< UI Ŭ����

	UPROPERTY(EditDefaultsOnly, Category = "SkillE")
		TSubclassOf<class ACSkill2Assassin> SkillEClass;		//<< E��ų Ŭ����


	uint8 Qidx = 0;									//<< Q��ų �ε���

	class ACharacter* TargetEnemy = nullptr;		//<< Q��ų Ÿ���� �ɸ���
	TArray<class ACharacter*> attackedTarget;		//<< �ǴϽ� ���� ���� �ɸ���

	bool bSkillQ = false;							//<< Q��ų ��������� üũ

	UCUserWidget_Target* TargetWidget;				//<< UI

/* =========================================== PRIVATE =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//

};
