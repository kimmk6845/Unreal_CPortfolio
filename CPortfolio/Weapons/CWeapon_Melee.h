#pragma once

#include "CoreMinimal.h"
#include "Weapons/CWeapon.h"
#include "CWeapon_Melee.generated.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 매크로 함수
//// TODO: 데이터에 맞는 애니메이션 재생, 공격 도중 움직임 제어, 카메라 제어
#define PLAY_ANIM_MONTAGE(index) \
	Character->PlayAnimMontage(DoActionDatas[index].DoActionMontage, DoActionDatas[index].DoAction_MontagePlayRatio);\
	if (DoActionDatas[index].bCanMove == false) Status->Stop();\
	if (DoActionDatas[index].bFixedCamera == true) Status->EnableFixedCamera();\
	else Status->DisableFixedCamera();

#define PLAY_ANIM_AIRMONTAGE(index) \
	Character->PlayAnimMontage(AirComboDatas[index].AirComboMontage, AirComboDatas[index].AirCombo_MontagePlayRatio);\
	if (AirComboDatas[index].bCanMove == false) Status->Stop();\
	if (AirComboDatas[index].bFixedCamera == true) Status->EnableFixedCamera();\
	else Status->DisableFixedCamera();
///////////////////////////////////////////////////////////////////////////////////////////////////


UCLASS()
class CPORTFOLIO_API ACWeapon_Melee : public ACWeapon
{
	GENERATED_BODY()

////  @ Default Class Function
///////////////////////////////////////////////////////////////////////////////////////////////////
public:
	ACWeapon_Melee();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
///////////////////////////////////////////////////////////////////////////////////////////////////


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* =========================================== PUBLIC =========================================== */

//////////////////////////////////
///// <<<<< FUNCTION >>>>>> //////
//////////////////////////////////
public:
	//// @ 콤보 변수 조작 함수(노티파이)
	////////////////////////////////////////////////////////////////////////////////
	void EnableCombo() { bEnable = true; }
	void DisableCombo() { bEnable = false; }
	////////////////////////////////////////////////////////////////////////////////


	//// @ 콜리전 범위 조작 함수
	////////////////////////////////////////////////////////////////////////////////
	virtual void OnRangeCollision();
	virtual void OffRangeCollision();
	////////////////////////////////////////////////////////////////////////////////


	//// @ LaunchCharacter 값 조작 함수
	////////////////////////////////////////////////////////////////////////////////
	void LaunchCharacter();
	void DownCharacter();
	////////////////////////////////////////////////////////////////////////////////


	//// @ 콜리전 On/Off 함수
	////////////////////////////////////////////////////////////////////////////////
	void OnCollision();
	void OffColiision();
	////////////////////////////////////////////////////////////////////////////////


/* =========================================== PUBLIC =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* ========================================== PROTECTED ========================================== */

//////////////////////////////////
///// <<<<< FUNCTION >>>>>> //////
//////////////////////////////////
protected:
	//// @ DoAction 상속 함수
	////////////////////////////////////////////////////////////////////////////////
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;
	void RandomDoAction() override;
	////////////////////////////////////////////////////////////////////////////////


	//// @ SubAction 상속 함수
	////////////////////////////////////////////////////////////////////////////////
	void Begin_SubAction() override;
	////////////////////////////////////////////////////////////////////////////////


	//// @ SubSkill 상속 함수
	////////////////////////////////////////////////////////////////////////////////
	virtual void SubSkill() override {}
	////////////////////////////////////////////////////////////////////////////////


	//// @ 백어택 함수
	////////////////////////////////////////////////////////////////////////////////
	virtual void BackAttack() {}
	////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////
///// <<<<< VARIABLE >>>>>> //////
//////////////////////////////////
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Collision")
		class UCapsuleComponent* Collision;					//<< 메시 캡슐 콜리전

	UPROPERTY(EditDefaultsOnly, Category = "Add-On")
		TSubclassOf<class ACGhostTrail> GhostTrailClass;	//<< 잔상 이펙트를 담을 클래스

	UPROPERTY(EditDefaultsOnly, Category = "NG_Dash")
		class UNiagaraSystem* NGDashEffect;					//<< 잔상 이펙트

	float CollisionRadius;		//<< 콜리전 범위 변수

	bool bInAir = false;		//<< 공중에 떠 있는지 체크
	bool bSkilling = false;		//<< 스킬 사용중인지 체크
	float InitSpringArmLength;	//<< 스프링암 조절하기 위한 초기 스프링암 값 담는 변수

	class ACharacter* Candidate;			//<< 멀티 히트 발생 시 케릭터의 가장 정면에 있는 적 케릭터
	class USpringArmComponent* springArm;	//<< 스프링암
	class UCameraComponent* camera;			//<< 카메라

/* ========================================== PROTECTED ========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* =========================================== PRIVATE =========================================== */

//////////////////////////////////
///// <<<<< FUNCTION >>>>>> //////
//////////////////////////////////
private:
	//// @ 콜리전 겹쳤을 시 데미지 전달할 함수
	////////////////////////////////////////////////////////////////////////////////
	void OnBeginOverlap(class ACharacter* otherCharacter, class UShapeComponent* attackComponent);
	////////////////////////////////////////////////////////////////////////////////


	//// @ 콜리전 Overlap 델리게이트 함수
	////////////////////////////////////////////////////////////////////////////////
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////
///// <<<<< VARIABLE >>>>>> //////
//////////////////////////////////
private:
	bool bEnable;	//<< 콤보가 가능한지 체크
	bool bExist;	//<< 다음 콤보가 있는지 체크

	int index = 0;		//<< 콤보 인덱스

	float OriginCollisionRadius;	//<< 초기 콜리전 범위

	TArray<class UShapeComponent*> Collisions;	//<< 자식클래스의 콜리전을 제어하기 위한 변수

	TArray<class ACharacter*> Hitted;			//<< 콜리전에 overlap된 케릭터들을 담을 변수
	
	class ACGhostTrail* GhostTrail;				//<< 잔상 효과 클래스 변수

/* =========================================== PRIVATE =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//

};
