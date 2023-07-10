#pragma once

#include "CoreMinimal.h"
#include "Weapons/CWeapon_Melee.h"
#include "CWeapon_Melee_Assassin.generated.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ CONST GLOBAL VARIABLE
const FVector DisableTeleportingLocation = FVector::ZeroVector + FVector(0, 0, 1e9); //<< 텔레포트 불가 지점
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
	//// @ 장착 및 해제 노티파이 함수
	////////////////////////////////////////////////////////////////////////////////
	void Begin_Equip() override;
	void End_UnEquip() override;
	////////////////////////////////////////////////////////////////////////////////
	
	
	//// @ Q 스킬 노티파이 함수
	////////////////////////////////////////////////////////////////////////////////
	void SkillQ() override;
	void Begin_SkillQ() override;
	void End_SkillQ() override;
	////////////////////////////////////////////////////////////////////////////////


	//// @ E 스킬 노티파이 함수
	////////////////////////////////////////////////////////////////////////////////
	void SkillE() override;
	void Begin_SkillE() override;
	void End_SkillE() override;
	////////////////////////////////////////////////////////////////////////////////


	//// @ 공중 콤보 백어택
	////////////////////////////////////////////////////////////////////////////////
	void BackAttack() override;
	////////////////////////////////////////////////////////////////////////////////


	//// @ SubSkill (Input X Key)
	////////////////////////////////////////////////////////////////////////////////
	void SubSkill() override;
	////////////////////////////////////////////////////////////////////////////////


	//// @ Q 스킬 텔레포트 위치 탐색 함수
	////////////////////////////////////////////////////////////////////////////////
	FVector TraceCanTeleportLocation(class ACharacter* target);
	bool TraceBeingFloor(FVector capsuleLocation);
	////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////
///// <<<<< VARIABLE >>>>>> //////
//////////////////////////////////
private:
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		class UStaticMeshComponent* Sword;				//<< 칼 메시

	UPROPERTY(EditDefaultsOnly, Category = "Skill Q")
		class UAnimMontage* SkillQAttackMontages[5];	//<< Q스킬 공격 애니메이션 몽타주 배열

	UPROPERTY(EditDefaultsOnly, Category = "Skill Q")
		class UAnimMontage* SkillQHitMontages[5];		//<< Q스킬 피격 애니메이션 몽타주 배열

	UPROPERTY(EditDefaultsOnly, Category = "Skill Q")
		float distanceToEnemy[5] = { 180.0f, 200.0f, 134.0f, 110.0f, 160.0f };	//<< Q스킬 적과의 거리

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
		TSubclassOf<class UCUserWidget_Target> TargetWidgetClass;	//<< UI 클래스

	UPROPERTY(EditDefaultsOnly, Category = "SkillE")
		TSubclassOf<class ACSkill2Assassin> SkillEClass;		//<< E스킬 클래스


	uint8 Qidx = 0;									//<< Q스킬 인덱스

	class ACharacter* TargetEnemy = nullptr;		//<< Q스킬 타게팅 케릭터
	TArray<class ACharacter*> attackedTarget;		//<< 피니시 공격 당한 케릭터

	bool bSkillQ = false;							//<< Q스킬 사용중인지 체크

	UCUserWidget_Target* TargetWidget;				//<< UI

/* =========================================== PRIVATE =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//

};
