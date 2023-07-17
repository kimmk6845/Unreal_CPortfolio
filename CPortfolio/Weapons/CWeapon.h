#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IDamage.h"
#include "CWeapon.generated.h"


UCLASS()
class CPORTFOLIO_API ACWeapon : public AActor
{
	GENERATED_BODY()

////  @ Default Class Function
///////////////////////////////////////////////////////////////////////////////////////////////////
public:
	ACWeapon();

protected:
	virtual void BeginPlay() override;

///////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////
// <<<< Data & DataTable >>>> ///
//////////////////////////////////

//// @ DoAction & DoAirAction
///////////////////////////////////////////////////////////////////////////////////////////////////
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		TArray<FDoActionDatas>  DoActionDatas;

	UPROPERTY(BlueprintReadWrite, Category = "Data")
		TArray<FHitDatas>  HitDatas;

	UPROPERTY(BlueprintReadWrite, Category = "Data")
		TArray<FAirComboDatas>  AirComboDatas;

	UPROPERTY(BlueprintReadWrite, Category = "Data")
		TArray<FHitDatas>  AirHitDatas;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "DataTable")
		class UDataTable* DoActionDataTable;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "DataTable")
		class UDataTable* HitDataTable;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "DataTable")
		class UDataTable* AirComboDataTable;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "DataTable")
		class UDataTable* AirHitDataTable;
///////////////////////////////////////////////////////////////////////////////////////////////////


//// @ Skill
///////////////////////////////////////////////////////////////////////////////////////////////////
public:
	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FHitDatas SkillQHitData;				// ������ �ϳ�

	UPROPERTY(BlueprintReadWrite, Category = "Data")
		TArray<FHitDatas> SkillEHitDatas;		// ������ ���� ��

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "DataTable")
		class UDataTable* SkillQHitDataTable;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "DataTable")
		class UDataTable* SkillEHitDataTable;
///////////////////////////////////////////////////////////////////////////////////////////////////


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* =========================================== PUBLIC =========================================== */

//////////////////////////////////
///// <<<<< FUNCTION >>>>>> //////
//////////////////////////////////

public:
	//// @ ���� ���� �� ����
	////////////////////////////////////////////////////////////////////////////////
	void Equip();
	virtual void Begin_Equip();
	virtual void End_Equip();
	void Unequip();
	virtual void End_UnEquip();
	////////////////////////////////////////////////////////////////////////////////


	//// @ Action (Left Click Function)
	////////////////////////////////////////////////////////////////////////////////
	virtual void DoAction() {}
	virtual void Begin_DoAction();
	virtual void End_DoAction();
	virtual void RandomDoAction() {}

	//// @ Rifle -> Left Click Released
	virtual void End_Fire() {}
	////////////////////////////////////////////////////////////////////////////////


	//// @ SubAction (Right Click Function)
	////////////////////////////////////////////////////////////////////////////////
	virtual void Begin_SubAction() {}
	virtual void End_SubAction() {}
	////////////////////////////////////////////////////////////////////////////////


	//// @ Q Skill Function (Input Q Key)
	////////////////////////////////////////////////////////////////////////////////
	virtual void SkillQ();
	virtual void End_SkillQ();
	virtual void Begin_SkillQ() {}
	////////////////////////////////////////////////////////////////////////////////


	//// @ E Skill Function (Input E Key)
	////////////////////////////////////////////////////////////////////////////////
	virtual void SkillE();
	virtual void End_SkillE();
	virtual void Begin_SkillE() {}
	////////////////////////////////////////////////////////////////////////////////


	//// @ SubSkill Function (Input X Key)
	////////////////////////////////////////////////////////////////////////////////
	virtual void SubSkill() {}
	////////////////////////////////////////////////////////////////////////////////


	//// @ Dash Function
	////////////////////////////////////////////////////////////////////////////////
	FORCEINLINE UAnimMontage* GetDashMontage() { return DashMontage; }
	FORCEINLINE float GetDashDistance() { return DashDistance; }
	////////////////////////////////////////////////////////////////////////////////


/* =========================================== PUBLIC =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* ========================================== PROTECTED ========================================== */

//////////////////////////////////
///// <<<<< VARIABLE >>>>>> //////
//////////////////////////////////
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		class UAnimMontage* EquipMontage;		//<< ���� �ִϸ��̼� ��Ÿ��

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		class UAnimMontage* UnEquipMontage;		//<< ���� �ִϸ��̼� ��Ÿ��

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		float Equip_MontagePlayRatio = 1.0f;	//<< ���� �� ���� �ִϸ��̼� �ӵ�

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		bool bUseControlRotation = true;		//<< ControlRotationYaw ���� ����

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		bool bCanMove = true;					//<< �̵� ����/�Ұ��� ���� ����

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		FName HolsterSocketName;				//<< ������ �� �ɸ��� Bone�� ���� ���� �̸�

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		FName EquipSocketName;					//<< ���� �� �ɸ��� Bone�� ���� ���� �̸�

	UPROPERTY(EditDefaultsOnly, Category = "SkillQ")
		class UAnimMontage* SkillQMontage = nullptr;	//<< ��ųQ �ִϸ��̼� ��Ÿ��

	UPROPERTY(EditDefaultsOnly, Category = "SkillE")
		class UAnimMontage* SkillEMontage = nullptr;	//<< ��ųE �ִϸ��̼� ��Ÿ��

	UPROPERTY(EditDefaultsOnly, Category = "SkillQ")
		float SkillQ_PlayRatio = 1.0f;					//<< ��ųQ �ִϸ��̼� �ӵ�

	UPROPERTY(EditDefaultsOnly, Category = "SkillE")
		float SkillE_PlayRatio = 1.0f;					//<< ��ųE �ִϸ��̼� �ӵ�

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
		class UAnimMontage* DashMontage;				//<< �뽬 �ִϸ��̼� ��Ÿ��

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
		float Dash_PlayRatio = 1.0f;					//<< �뽬 �ִϸ��̼� �ӵ�

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
		float DashDistance = 10.0f;						//<< �뽬 �Ÿ�


protected:
	class ACharacter* Character;				//<< Weapon ������ �ɸ���
	class UCStateComponent* State;				//<< �������� State ������Ʈ
	class UCStatusComponent* Status;			//<< �������� Status ������Ʈ
	class UCWeaponComponent* Weapon;			//<< �������� Weapon ������Ʈ

/* ========================================== PROTECTED ========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* =========================================== PRIVATE =========================================== */

//////////////////////////////////
///// <<<<< VARIABLE >>>>>> //////
//////////////////////////////////
private:
	bool bInEquipping;	//<< ���������� üũ
	bool bInEquip;		//<< �����Ǿ����� üũ
	bool bInAction;		//<< ���������� üũ

/* =========================================== PRIVATE =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
};
