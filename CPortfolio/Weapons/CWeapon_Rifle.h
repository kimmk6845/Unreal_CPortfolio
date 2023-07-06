#pragma once

#include "CoreMinimal.h"
#include "Weapons/CWeapon.h"
#include "CWeapon_Rifle.generated.h"

//// @ 기본&조준 에임 데이터 구조체
///////////////////////////////////////////////////////////////////////////////////////////////////
USTRUCT(BlueprintType)
struct FWeaponAimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float TargetArmLength;	//<< 스프링암 거리

	UPROPERTY(EditAnywhere)
		FVector SocketOffset;	//<< 스프링암의 socketOffset

	UPROPERTY(EditAnywhere)
		float FieldOfView;		//<< FOV

public:
	// @ 커브 값이 있을 때 Aim Data 로드
	void SetData(class ACharacter* InOwner);

	// @ 커브 값이 없을 때 Aim Data 로드
	void SetDataByNoneCurve(class ACharacter* InOwner);
};
///////////////////////////////////////////////////////////////////////////////////////////////////


UCLASS()
class CPORTFOLIO_API ACWeapon_Rifle : public ACWeapon
{
	GENERATED_BODY()

//// @ Default Class Function
///////////////////////////////////////////////////////////////////////////////////////////////////
public:
	ACWeapon_Rifle();

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
	// @ Getter
	////////////////////////////////////////////////////////////////////////////////
	FORCEINLINE bool IsInAim() { return bInAim; }
	FORCEINLINE bool IsFiring() { return bFiring; }
	FORCEINLINE bool IsAutoFire() { return bAutoFire; }

	FORCEINLINE FVector GetLeftHandLocation() { return LeftHandLocation; }

	FORCEINLINE uint8 GetCurrMagazineCount() { return CurrMagazineCount; }
	FORCEINLINE uint8 GetMaxMagazineCount() { return MaxMagazineCount; }
	////////////////////////////////////////////////////////////////////////////////


	// @ 장착 관련 함수
	////////////////////////////////////////////////////////////////////////////////
	bool CanEquip();
	void Begin_Equip() override;
	void End_Equip() override;
	////////////////////////////////////////////////////////////////////////////////


	// @ 해제 관련 함수
	////////////////////////////////////////////////////////////////////////////////
	bool CanUnequip();
	void End_UnEquip() override;
	////////////////////////////////////////////////////////////////////////////////


	// @ DoAction 관련 함수
	////////////////////////////////////////////////////////////////////////////////
	bool CanFire();
	void Begin_Fire();
	void End_Fire() override;
	////////////////////////////////////////////////////////////////////////////////


	// @ Reload 관련 함수
	////////////////////////////////////////////////////////////////////////////////
	bool CanReload();
	void Reload();

	void Eject_Magazine();
	void Spawn_Magazine();
	void Load_Magazine();
	void End_Reload();
	////////////////////////////////////////////////////////////////////////////////


	// @ 연사 toggle 함수
	////////////////////////////////////////////////////////////////////////////////
	void ToggleAutoFire();
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
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;				//<< 루트 씬 컴포넌트

	UPROPERTY(VisibleDefaultsOnly)
		class USkeletalMeshComponent* Mesh;			//<< 스켈레탈 메시(총)

	UPROPERTY(EditDefaultsOnly, Category = "Data")
		FWeaponAimData BaseData;					//<< 기본 Pose에 대한 AimData

	UPROPERTY(EditDefaultsOnly, Category = "Data")
		FWeaponAimData AimData;						//<< 조준 Pose에 대한 AimData

	UPROPERTY(EditDefaultsOnly, Category = "Data")
		FVector LeftHandLocation;					//<< Hand IK 왼손 위치

	UPROPERTY(EditDefaultsOnly, Category = "Aiming")
		class UCurveFloat* AimCurve;				//<< Pose 변경에 대한 커브 값

	UPROPERTY(EditDefaultsOnly, Category = "Aiming")
		float AimingSpeed = 200;					//<< 조준 속도

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		uint8 MaxMagazineCount = 30;				//<< 최대 탄창

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		class UAnimMontage* ReloadMontage;			//<< 재장전 애니메이션 몽타주

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		FName MagazineBoneName;						//<< 탄창 Bone 이름

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		TSubclassOf<class ACMagazine> MagazineClass;	//<< 탄창 클래스 (재장전 시 떨어지는 탄창을 위함)

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		FName MagazineAttachSocketName;				//<< 탄창이 위치한 소켓 이름

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		FName EjectBoneName;						//<< 탄피가 나올 Bone 이름

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		class UParticleSystem* EjectParticle;		//<< 탄피 파티클

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		FName MuzzleBoneName;						//<< 총구 Bone 이름

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		class UParticleSystem* FlashParticle;		//<< 총구에서의 사격 파티클

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		class USoundWave* FireSound;				//<< 사격 사운드

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		TSubclassOf<class ACBullet> BulletClass;	//<< 날아갈 총알 클래스

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		TSubclassOf<class UMatineeCameraShake> CameraShakeClass;	//<< 반동을 위한 카메라 쉐이크 클래스

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		float AutoFireInterval = 0.125f;			//<< 연사 속도

	UPROPERTY(EditDefaultsOnly, Category = "Recoil")
		float RecoilAngle = 0.05;					//<< 탄 퍼짐

	UPROPERTY(EditDefaultsOnly, Category = "Recoil")
		float RecoilRate = 0.1f;					//<< 반동

	UPROPERTY(EditDefaultsOnly, Category = "Recoil")
		float SpreadSpeed = 2.0f;					//<< 크로스헤어 UI 퍼짐 속도

	UPROPERTY(EditDefaultsOnly, Category = "Recoil")
		float MaxSpreadAlignment = 2.0f;			//<< 크로스헤어 UI 최대 퍼짐 비율

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		float HitDistance = 3000;					//<< 최대 사거리

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
		class UMaterialInstanceConstant* HitDecal;	//<< 피격 부분의 Decal

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
		class UParticleSystem* HitParticle;			//<< 피격 지형에 대한 파티클

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
		class UNiagaraSystem* HitNiagara;			//<< 피격 액터에 대한 나이아가라

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
		TSubclassOf<class UCUserWidget_CrossHair> CrossHairClass;	//<< UI 클래스

	UCUserWidget_CrossHair* CrossHair;				//<< UI


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
	//// @ DoAction 상속 함수
	////////////////////////////////////////////////////////////////////////////////
	void DoAction() override;
	////////////////////////////////////////////////////////////////////////////////


	//// @ 조준 가능 여부 체크 함수
	////////////////////////////////////////////////////////////////////////////////
	bool CanAim();
	////////////////////////////////////////////////////////////////////////////////


	//// @ SubAction 노티파이 함수
	////////////////////////////////////////////////////////////////////////////////
	void Begin_SubAction() override;
	void End_SubAction() override;
	////////////////////////////////////////////////////////////////////////////////


	//// @ 조준 함수
	////////////////////////////////////////////////////////////////////////////////
	void OnAiming(float Output);
	////////////////////////////////////////////////////////////////////////////////


	//// @ Q 스킬 노티파이 함수
	////////////////////////////////////////////////////////////////////////////////
	void Begin_SkillQ() override;
	////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////
///// <<<<< VARIABLE >>>>>> //////
//////////////////////////////////
private:
	UPROPERTY(EditDefaultsOnly, Category = "SkillQ")
		TSubclassOf<class ACSkill1Rifle> SkillQClass;	//<< Q 스킬 스폰 클래스

	UPROPERTY(VisibleDefaultsOnly)
		class UTimelineComponent* Timeline;				//<< 커브 값 적용을 위한 타임라인

	bool bEquipping;				//<< 장착중 체크

	bool bInAim;					//<< 견착 체크
	bool bFiring;
	bool bReload;					//<< 재장전중 체크
	bool bAutoFire = true;			//<< 연사모드 체크

	FTimerHandle AutoFireHandle;	//<< 타임라인 핸들러

	float CurrSpreadRadius;			//<< 현재 UI퍼짐 반지름
	float LastAddSpreadTime;		//<< 마지막 사격 시간

	float CurrRecoil;				//<< 현재 탄퍼짐

	uint8 CurrMagazineCount;		//<< 현재 탄알 수
	class ACMagazine* Magazine;		//<< 탄창 클래스

	class UCameraComponent* Camera;			//<< 카메라 컴포넌트
	class USpringArmComponent* SpringArm;	//<< 스프링암 컴포넌트

/* =========================================== PRIVATE =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//

};
