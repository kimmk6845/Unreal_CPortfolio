#pragma once

#include "CoreMinimal.h"
#include "Weapons/CWeapon_Melee.h"
#include "CWeapon_Melee_OneHand.generated.h"

UCLASS()
class CPORTFOLIO_API ACWeapon_Melee_OneHand : public ACWeapon_Melee
{
	GENERATED_BODY()

////  @ Default Class Function
///////////////////////////////////////////////////////////////////////////////////////////////////
public:
	ACWeapon_Melee_OneHand();

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


	//// @ Q��ų ��Ƽ���� �Լ�
	////////////////////////////////////////////////////////////////////////////////
	void Begin_SkillQ() override;
	////////////////////////////////////////////////////////////////////////////////


	//// @ E ��ų ��Ƽ���� �Լ�
	////////////////////////////////////////////////////////////////////////////////
	void SkillE() override;
	void Begin_SkillE() override;
	void End_SkillE() override;
	////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////
///// <<<<< VARIABLE >>>>>> //////
//////////////////////////////////
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		class UStaticMeshComponent* Sheath;					//<< Į��

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		class UStaticMeshComponent* Sword;					//<< Į �޽�

	UPROPERTY(EditDefaultsOnly, Category = "SkillQ")
		TSubclassOf<class ACSkill1OneHand> SkillQClass;		//<< ��ų ����ü Ŭ����

	UPROPERTY(EditDefaultsOnly, Category = "SkillE")
		TSubclassOf<class ACSkill2OneHand> SkillEClass;	//<< E��ų Ŭ����

	class ACSkill2OneHand* skillE;	//<< ���� E��ų ����Ŭ����

/* =========================================== PRIVATE =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//

};
