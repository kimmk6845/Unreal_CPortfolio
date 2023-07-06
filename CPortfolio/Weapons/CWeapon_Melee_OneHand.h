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
	//// @ 장착 및 해제 노티파이 함수
	////////////////////////////////////////////////////////////////////////////////
	void Begin_Equip() override;
	void End_UnEquip() override;
	////////////////////////////////////////////////////////////////////////////////


	//// @ Q스킬 노티파이 함수
	////////////////////////////////////////////////////////////////////////////////
	void Begin_SkillQ() override;
	////////////////////////////////////////////////////////////////////////////////


	//// @ E 스킬 노티파이 함수
	////////////////////////////////////////////////////////////////////////////////
	void SkillE() override;
	void Begin_SkillE() override;
	void End_SkillE() override;
	////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////
///// <<<<< VARIABLE >>>>>> //////
//////////////////////////////////
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		class UStaticMeshComponent* Sheath;					//<< 칼집

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		class UStaticMeshComponent* Sword;					//<< 칼 메시

	UPROPERTY(EditDefaultsOnly, Category = "SkillQ")
		TSubclassOf<class ACSkill1OneHand> SkillQClass;		//<< 스킬 투사체 클래스

	UPROPERTY(EditDefaultsOnly, Category = "SkillE")
		TSubclassOf<class ACSkill2OneHand> SkillEClass;	//<< E스킬 클래스

	class ACSkill2OneHand* skillE;	//<< 스폰 E스킬 액터클래스

/* =========================================== PRIVATE =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//

};
