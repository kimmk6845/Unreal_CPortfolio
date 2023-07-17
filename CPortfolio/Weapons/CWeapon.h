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
		FHitDatas SkillQHitData;				// 데이터 하나

	UPROPERTY(BlueprintReadWrite, Category = "Data")
		TArray<FHitDatas> SkillEHitDatas;		// 데이터 여러 개

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
	//// @ 무기 장착 및 해제
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
		class UAnimMontage* EquipMontage;		//<< 장착 애니메이션 몽타주

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		class UAnimMontage* UnEquipMontage;		//<< 해제 애니메이션 몽타주

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		float Equip_MontagePlayRatio = 1.0f;	//<< 장착 및 해제 애니메이션 속도

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		bool bUseControlRotation = true;		//<< ControlRotationYaw 조작 변수

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		bool bCanMove = true;					//<< 이동 가능/불가능 조작 변수

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		FName HolsterSocketName;				//<< 미장착 시 케릭터 Bone에 붙을 소켓 이름

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		FName EquipSocketName;					//<< 장착 시 케릭터 Bone에 붙을 소켓 이름

	UPROPERTY(EditDefaultsOnly, Category = "SkillQ")
		class UAnimMontage* SkillQMontage = nullptr;	//<< 스킬Q 애니메이션 몽타주

	UPROPERTY(EditDefaultsOnly, Category = "SkillE")
		class UAnimMontage* SkillEMontage = nullptr;	//<< 스킬E 애니메이션 몽타주

	UPROPERTY(EditDefaultsOnly, Category = "SkillQ")
		float SkillQ_PlayRatio = 1.0f;					//<< 스킬Q 애니메이션 속도

	UPROPERTY(EditDefaultsOnly, Category = "SkillE")
		float SkillE_PlayRatio = 1.0f;					//<< 스킬E 애니메이션 속도

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
		class UAnimMontage* DashMontage;				//<< 대쉬 애니메이션 몽타주

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
		float Dash_PlayRatio = 1.0f;					//<< 대쉬 애니메이션 속도

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
		float DashDistance = 10.0f;						//<< 대쉬 거리


protected:
	class ACharacter* Character;				//<< Weapon 소유주 케릭터
	class UCStateComponent* State;				//<< 소유주의 State 컴포넌트
	class UCStatusComponent* Status;			//<< 소유주의 Status 컴포넌트
	class UCWeaponComponent* Weapon;			//<< 소유주의 Weapon 컴포넌트

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
	bool bInEquipping;	//<< 장착중인지 체크
	bool bInEquip;		//<< 장착되었는지 체크
	bool bInAction;		//<< 공격중인지 체크

/* =========================================== PRIVATE =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
};
