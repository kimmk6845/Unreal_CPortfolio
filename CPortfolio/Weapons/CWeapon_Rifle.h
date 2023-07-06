#pragma once

#include "CoreMinimal.h"
#include "Weapons/CWeapon.h"
#include "CWeapon_Rifle.generated.h"

//// @ �⺻&���� ���� ������ ����ü
///////////////////////////////////////////////////////////////////////////////////////////////////
USTRUCT(BlueprintType)
struct FWeaponAimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float TargetArmLength;	//<< �������� �Ÿ�

	UPROPERTY(EditAnywhere)
		FVector SocketOffset;	//<< ���������� socketOffset

	UPROPERTY(EditAnywhere)
		float FieldOfView;		//<< FOV

public:
	// @ Ŀ�� ���� ���� �� Aim Data �ε�
	void SetData(class ACharacter* InOwner);

	// @ Ŀ�� ���� ���� �� Aim Data �ε�
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


	// @ ���� ���� �Լ�
	////////////////////////////////////////////////////////////////////////////////
	bool CanEquip();
	void Begin_Equip() override;
	void End_Equip() override;
	////////////////////////////////////////////////////////////////////////////////


	// @ ���� ���� �Լ�
	////////////////////////////////////////////////////////////////////////////////
	bool CanUnequip();
	void End_UnEquip() override;
	////////////////////////////////////////////////////////////////////////////////


	// @ DoAction ���� �Լ�
	////////////////////////////////////////////////////////////////////////////////
	bool CanFire();
	void Begin_Fire();
	void End_Fire() override;
	////////////////////////////////////////////////////////////////////////////////


	// @ Reload ���� �Լ�
	////////////////////////////////////////////////////////////////////////////////
	bool CanReload();
	void Reload();

	void Eject_Magazine();
	void Spawn_Magazine();
	void Load_Magazine();
	void End_Reload();
	////////////////////////////////////////////////////////////////////////////////


	// @ ���� toggle �Լ�
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
		class USceneComponent* Root;				//<< ��Ʈ �� ������Ʈ

	UPROPERTY(VisibleDefaultsOnly)
		class USkeletalMeshComponent* Mesh;			//<< ���̷�Ż �޽�(��)

	UPROPERTY(EditDefaultsOnly, Category = "Data")
		FWeaponAimData BaseData;					//<< �⺻ Pose�� ���� AimData

	UPROPERTY(EditDefaultsOnly, Category = "Data")
		FWeaponAimData AimData;						//<< ���� Pose�� ���� AimData

	UPROPERTY(EditDefaultsOnly, Category = "Data")
		FVector LeftHandLocation;					//<< Hand IK �޼� ��ġ

	UPROPERTY(EditDefaultsOnly, Category = "Aiming")
		class UCurveFloat* AimCurve;				//<< Pose ���濡 ���� Ŀ�� ��

	UPROPERTY(EditDefaultsOnly, Category = "Aiming")
		float AimingSpeed = 200;					//<< ���� �ӵ�

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		uint8 MaxMagazineCount = 30;				//<< �ִ� źâ

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		class UAnimMontage* ReloadMontage;			//<< ������ �ִϸ��̼� ��Ÿ��

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		FName MagazineBoneName;						//<< źâ Bone �̸�

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		TSubclassOf<class ACMagazine> MagazineClass;	//<< źâ Ŭ���� (������ �� �������� źâ�� ����)

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		FName MagazineAttachSocketName;				//<< źâ�� ��ġ�� ���� �̸�

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		FName EjectBoneName;						//<< ź�ǰ� ���� Bone �̸�

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		class UParticleSystem* EjectParticle;		//<< ź�� ��ƼŬ

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		FName MuzzleBoneName;						//<< �ѱ� Bone �̸�

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		class UParticleSystem* FlashParticle;		//<< �ѱ������� ��� ��ƼŬ

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		class USoundWave* FireSound;				//<< ��� ����

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		TSubclassOf<class ACBullet> BulletClass;	//<< ���ư� �Ѿ� Ŭ����

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		TSubclassOf<class UMatineeCameraShake> CameraShakeClass;	//<< �ݵ��� ���� ī�޶� ����ũ Ŭ����

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		float AutoFireInterval = 0.125f;			//<< ���� �ӵ�

	UPROPERTY(EditDefaultsOnly, Category = "Recoil")
		float RecoilAngle = 0.05;					//<< ź ����

	UPROPERTY(EditDefaultsOnly, Category = "Recoil")
		float RecoilRate = 0.1f;					//<< �ݵ�

	UPROPERTY(EditDefaultsOnly, Category = "Recoil")
		float SpreadSpeed = 2.0f;					//<< ũ�ν���� UI ���� �ӵ�

	UPROPERTY(EditDefaultsOnly, Category = "Recoil")
		float MaxSpreadAlignment = 2.0f;			//<< ũ�ν���� UI �ִ� ���� ����

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
		float HitDistance = 3000;					//<< �ִ� ��Ÿ�

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
		class UMaterialInstanceConstant* HitDecal;	//<< �ǰ� �κ��� Decal

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
		class UParticleSystem* HitParticle;			//<< �ǰ� ������ ���� ��ƼŬ

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
		class UNiagaraSystem* HitNiagara;			//<< �ǰ� ���Ϳ� ���� ���̾ư���

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
		TSubclassOf<class UCUserWidget_CrossHair> CrossHairClass;	//<< UI Ŭ����

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
	//// @ DoAction ��� �Լ�
	////////////////////////////////////////////////////////////////////////////////
	void DoAction() override;
	////////////////////////////////////////////////////////////////////////////////


	//// @ ���� ���� ���� üũ �Լ�
	////////////////////////////////////////////////////////////////////////////////
	bool CanAim();
	////////////////////////////////////////////////////////////////////////////////


	//// @ SubAction ��Ƽ���� �Լ�
	////////////////////////////////////////////////////////////////////////////////
	void Begin_SubAction() override;
	void End_SubAction() override;
	////////////////////////////////////////////////////////////////////////////////


	//// @ ���� �Լ�
	////////////////////////////////////////////////////////////////////////////////
	void OnAiming(float Output);
	////////////////////////////////////////////////////////////////////////////////


	//// @ Q ��ų ��Ƽ���� �Լ�
	////////////////////////////////////////////////////////////////////////////////
	void Begin_SkillQ() override;
	////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////
///// <<<<< VARIABLE >>>>>> //////
//////////////////////////////////
private:
	UPROPERTY(EditDefaultsOnly, Category = "SkillQ")
		TSubclassOf<class ACSkill1Rifle> SkillQClass;	//<< Q ��ų ���� Ŭ����

	UPROPERTY(VisibleDefaultsOnly)
		class UTimelineComponent* Timeline;				//<< Ŀ�� �� ������ ���� Ÿ�Ӷ���

	bool bEquipping;				//<< ������ üũ

	bool bInAim;					//<< ���� üũ
	bool bFiring;
	bool bReload;					//<< �������� üũ
	bool bAutoFire = true;			//<< ������ üũ

	FTimerHandle AutoFireHandle;	//<< Ÿ�Ӷ��� �ڵ鷯

	float CurrSpreadRadius;			//<< ���� UI���� ������
	float LastAddSpreadTime;		//<< ������ ��� �ð�

	float CurrRecoil;				//<< ���� ź����

	uint8 CurrMagazineCount;		//<< ���� ź�� ��
	class ACMagazine* Magazine;		//<< źâ Ŭ����

	class UCameraComponent* Camera;			//<< ī�޶� ������Ʈ
	class USpringArmComponent* SpringArm;	//<< �������� ������Ʈ

/* =========================================== PRIVATE =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//

};
