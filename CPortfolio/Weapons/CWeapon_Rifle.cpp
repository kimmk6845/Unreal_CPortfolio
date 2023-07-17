#include "Weapons/CWeapon_Rifle.h"
#include "Global.h"
#include "CBullet.h"
#include "CMagazine.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraShake.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Widgets/CUserWidget_CrossHair.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundWave.h"
#include "CSkill1Rifle.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Ŀ�� ���� ���� �� AimData �ε�
//// TODO: �������� ���� �� ��ġ ����
void FWeaponAimData::SetData(ACharacter* InOwner)
{
	USpringArmComponent* springArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	springArm->TargetArmLength = TargetArmLength;
	springArm->SocketOffset = SocketOffset;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Ŀ�� ���� ���� �� AimData �ε�
//// TODO: �������� ���� �� ��ġ ����, ī�޶� FOV ����
void FWeaponAimData::SetDataByNoneCurve(ACharacter* InOwner)
{
	USpringArmComponent* springArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	springArm->TargetArmLength = TargetArmLength;
	springArm->SocketOffset = SocketOffset;

	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(InOwner);
	camera->FieldOfView = FieldOfView;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ������
ACWeapon_Rifle::ACWeapon_Rifle()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh", Root);

	CHelpers::CreateActorComponent<UTimelineComponent>(this, &Timeline, "Timeline");
	CHelpers::GetAsset<UCurveFloat>(&AimCurve, "CurveFloat'/Game/CV_Aim.CV_Aim'");

	CHelpers::GetAsset<UMaterialInstanceConstant>(&HitDecal, "MaterialInstanceConstant'/Game/Materials/M_Decal_Inst.M_Decal_Inst'");
	CHelpers::GetAsset<UParticleSystem>(&HitParticle, "ParticleSystem'/Game/Effects/P_Impact_Default.P_Impact_Default'");
	CHelpers::GetAsset<UNiagaraSystem>(&HitNiagara, "NiagaraSystem'/Game/Effects/NG_Hit.NG_Hit'");
	
	CHelpers::GetClass<UCUserWidget_CrossHair>(&CrossHairClass, "WidgetBlueprint'/Game/BP/Widgets/WB_CrossHair.WB_CrossHair_C'");

	CHelpers::GetAsset<UParticleSystem>(&EjectParticle, "ParticleSystem'/Game/Effects/P_Eject_bullet.P_Eject_bullet'");
	CHelpers::GetAsset<UParticleSystem>(&FlashParticle, "ParticleSystem'/Game/Effects/P_Muzzleflash.P_Muzzleflash'");
	CHelpers::GetAsset<USoundWave>(&FireSound, "SoundWave'/Game/Sounds/S_RifleShoot.S_RifleShoot'");
	CHelpers::GetClass<UMatineeCameraShake>(&CameraShakeClass, "Blueprint'/Game/BP/Weapons/Rifle/BP_Rifle_CameraShake.BP_Rifle_CameraShake_C'");

	CHelpers::GetClass<ACBullet>(&BulletClass, "Blueprint'/Game/BP/Weapons/Rifle/BP_CBullet.BP_CBullet_C'");
	CHelpers::GetClass<ACMagazine>(&MagazineClass, "Blueprint'/Game/BP/Weapons/Rifle/BP_CMagazine.BP_CMagazine_C'");

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/WeaponMesh/Rifle/SK_AR4.SK_AR4'");
	Mesh->SetSkeletalMesh(mesh);

	HolsterSocketName = "Rifle_Holster";
	EquipSocketName = "Rifle_RightHand";

	CHelpers::GetAsset<UAnimMontage>(&EquipMontage, "AnimMontage'/Game/GhostLady_S1/Montages/Rifle/Rifle_Equip_Montage.Rifle_Equip_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&UnEquipMontage, "AnimMontage'/Game/GhostLady_S1/Montages/Rifle/Rifle_UnEquip_Montage.Rifle_UnEquip_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&ReloadMontage, "AnimMontage'/Game/GhostLady_S1/Montages/Rifle/Rifle_Reload_Montage.Rifle_Reload_Montage'");

	Equip_MontagePlayRatio = 1.5f;

	CHelpers::GetAsset<UDataTable>(&HitDataTable, L"DataTable'/Game/BP/Weapons/Rifle/Rifle_HitData.Rifle_HitData'");

	BaseData.TargetArmLength = 300;
	BaseData.SocketOffset = FVector(0, 60, 0);
	BaseData.FieldOfView = 90;

	AimData.TargetArmLength = 80;
	AimData.SocketOffset = FVector(0, 40, -5);
	AimData.FieldOfView = 65;

	MagazineBoneName = "b_gun_mag";
	MagazineAttachSocketName = "Rifle_Magazine";
	EjectBoneName = "b_gun_shelleject";
	MuzzleBoneName = "Muzzle";

	LeftHandLocation = FVector(-27, 10.5f, 5);

	CHelpers::GetClass(&SkillQClass, "Blueprint'/Game/BP/Weapons/Rifle/BP_CSkill1Rifle.BP_CSkill1Rifle_C'");

	CHelpers::GetAsset<UAnimMontage>(&SkillQMontage, "AnimMontage'/Game/GhostLady_S1/Montages/Rifle/Rifle_SkillQ_Montage.Rifle_SkillQ_Montage'");
	SkillQ_PlayRatio = 2.0f;

	CHelpers::GetAsset<UAnimMontage>(&DashMontage, L"AnimMontage'/Game/GhostLady_S1/Montages/Common/Dash_Rifle_Montage.Dash_Rifle_Montage'");
	DashDistance = 5.0f;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Begin Play
//// TODO: Ÿ�Ӷ��ο� ���� �Լ� ���, UI �ε�, �� �޽ø� �ɸ��� ���Ͽ� ����
void ACWeapon_Rifle::BeginPlay()
{
	Super::BeginPlay();

	CheckNull(Character);
	Camera = CHelpers::GetComponent<UCameraComponent>(Character);
	SpringArm = CHelpers::GetComponent<USpringArmComponent>(Character);

	BaseData.SetDataByNoneCurve(Character);


	if (!!AimCurve)
	{
		FOnTimelineFloat timeline;
		timeline.BindUFunction(this, "OnAiming");

		Timeline->AddInterpFloat(AimCurve, timeline);
		Timeline->SetLooping(false);
		Timeline->SetPlayRate(AimingSpeed);
	}

	if (!!CrossHairClass)
	{
		CrossHair = CreateWidget<UCUserWidget_CrossHair, APlayerController>(Character->GetController<APlayerController>(), CrossHairClass);
		CrossHair->AddToViewport();
		CrossHair->SetVisibility(ESlateVisibility::Hidden);
	}

	CurrMagazineCount = MaxMagazineCount;
	
	if (HolsterSocketName.IsValid())
		AttachToComponent(Character->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HolsterSocketName);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Tick
//// TODO: ���� �ð��� ������ ��� �ð��� ���� ���� �ð��� ������ �ݵ�, ź����, UI �ʱ�ȭ
void ACWeapon_Rifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LastAddSpreadTime >= 0.0f)
	{
		if (GetWorld()->GetTimeSeconds() - LastAddSpreadTime >= AutoFireInterval + 0.25f)
		{
			CurrSpreadRadius = 0;
			LastAddSpreadTime = 0;

			CurrRecoil = RecoilAngle;

			if (!!CrossHair)
				CrossHair->UpdateSpreadRange(CurrSpreadRadius, MaxSpreadAlignment);
		}
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////////


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* =========================================== PUBLIC =========================================== */

///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ���� �� �� �ִ��� üũ�ϴ� �Լ�
//// TODO: ������, ������, ��� ������ ������ üũ�� ��� �ƴϸ� true�� ����
bool ACWeapon_Rifle::CanEquip()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;

	return !b;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ���� ���� ��Ƽ���� �Լ�
//// TODO: �� �޽ø� �ɸ��� �� ��ġ�� ���Ͽ� ����
void ACWeapon_Rifle::Begin_Equip()
{
	Super::Begin_Equip();

	if (!!CrossHair)
		CrossHair->SetVisibility(ESlateVisibility::Visible);

	if (EquipSocketName.IsValid())
		AttachToComponent(Character->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), EquipSocketName);
}
void ACWeapon_Rifle::End_Equip()
{
	Super::End_Equip();

	bEquipping = false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ���� �� �� �ִ��� üũ�ϴ� �Լ�
//// TODO: ���� ������, ������ ������ Ȯ���ϰ� ��� �ƴϸ� true�� ��ȯ
bool ACWeapon_Rifle::CanUnequip()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;

	return !b;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ���� ���� ��Ƽ���� �Լ�
//// TODO: ���� �� ������̿��ٸ�, �̸� �����ϰ� ����� �� �޽õ� �ɸ����� �� �� ���Ͽ� ����. UI ����
void ACWeapon_Rifle::End_UnEquip()
{
	Super::End_UnEquip();

	End_SubAction();
	End_Fire();
	End_DoAction();

	if (HolsterSocketName.IsValid())
		AttachToComponent(Character->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HolsterSocketName);

	if (!!CrossHair)
		CrossHair->SetVisibility(ESlateVisibility::Hidden);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ����� �� �ִ��� üũ�ϴ� �Լ�
//// TODO: ����������, ������������, ��������� Ȯ���� ��� �ƴϸ� true�� ��ȯ
bool ACWeapon_Rifle::CanFire()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;	// ���縦 ����

	return !b;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ DoAction ���� �Լ�
//// TODO: �ѹ��� �Ѿ��� ���ư��� �ڵ�
//// TODO: �Ѿ� Ŭ������ �̿��� ���ư��� �Ѿ��� �����, �̴� ������ �ƹ� ������ ���� ����
//// TODO: LineTraceChaanel�� �̿��� ���������� Ÿ���� �Ѿ��� ����. Ÿ�ݵ� ��ġ�� ȿ�� �ο�
//// TODO: �ɸ��Ͱ� �ǰݵǸ� �ǰ� �������� ������ ����ȭ, �ǰ� ȿ�� �ο�
void ACWeapon_Rifle::Begin_Fire()
{
	// �Ѿ� Ŭ������ �׳� ���ư��� ���̴� ȿ��. ���������� Ÿ���� �����°� LineTrace�� �̿���
	if (State->IsJumpMode() == true)
	{
		End_Fire();
		return;
	}

	State->SetActionMode();

	FVector direction = Camera->GetForwardVector();
	FTransform transform = Camera->GetComponentToWorld();

	FVector start = transform.GetLocation() + direction;
	
	// ź ����
	if (bInAim)
		CurrRecoil = CurrRecoil + RecoilAngle / 2;
	else
		CurrRecoil = CurrRecoil + RecoilAngle;

	direction = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(direction, CurrRecoil);
	FVector end = transform.GetLocation() + direction * HitDistance;


	TArray<AActor*> ignores;
	ignores.Add(Character);

	FHitResult hitResult;	
	FCollisionQueryParams params;
	params.AddIgnoredActor(Character);
	GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, params);
	//UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery6, false, ignores, EDrawDebugTrace::None, hitResult, true);		// Trace�� Pawn
	if (hitResult.bBlockingHit)
	{
		ACharacter* hitCharacter = Cast<ACharacter>(hitResult.GetActor());
		if (!!hitCharacter)		// �ɸ��� �ǰ�
		{
			UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(hitCharacter);
			CheckNull(weapon);

			if (!!HitNiagara)
			{
				FRotator impactRotation = UKismetMathLibrary::FindLookAtRotation(hitResult.Location, hitResult.TraceStart);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitNiagara, hitResult.Location, impactRotation);
							
				/// ������ ó��
				float rangeDamage = FMath::FRandRange(5, 10);
				float applyDamage = HitDatas[0].Damage;
				if (hitResult.BoneName == "head")
					HitDatas[0].Damage = applyDamage * 3 - rangeDamage;
				else if(hitResult.BoneName == "spine_02")
					HitDatas[0].Damage = applyDamage * 1.5f - rangeDamage;
				else
					HitDatas[0].Damage = applyDamage - rangeDamage;

				weapon->Damaged(Character, this, &HitDatas[0]);
				HitDatas[0].Damage = applyDamage;
			}
		}		
		else	// ������ ���� ��
		{
			FRotator rotator = hitResult.ImpactNormal.Rotation();

			if (!!HitDecal)
			{
				UDecalComponent* decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), HitDecal, FVector(5), hitResult.Location, rotator, 10);
				decal->SetFadeScreenSize(0);
			}

			if (!!HitParticle)
			{
				FRotator impactRotation = UKismetMathLibrary::FindLookAtRotation(hitResult.Location, hitResult.TraceStart);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, hitResult.Location, impactRotation);
			}
		}	// if (!!hitCharacter)
	}	// if (hitResult.bBlockingHit)


	if (!!FlashParticle)
		UGameplayStatics::SpawnEmitterAttached(FlashParticle, Mesh, MuzzleBoneName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);

	if (!!EjectParticle)
		UGameplayStatics::SpawnEmitterAttached(EjectParticle, Mesh, EjectBoneName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);


	FVector muzzleLocation = Mesh->GetSocketLocation(MuzzleBoneName);

	if (!!FireSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, muzzleLocation);

	if (!!BulletClass)
	{
		FVector spawnLocation = muzzleLocation + direction * 100;

		ACBullet* bullet = GetWorld()->SpawnActor<ACBullet>(BulletClass, spawnLocation, direction.Rotation());
		bullet->Shoot(direction);
	}

	// �ݵ�
	if (bInAim)
		Character->AddControllerPitchInput(-RecoilRate * UKismetMathLibrary::RandomFloatInRange(0.4f, 0.6f));
	else
		Character->AddControllerPitchInput(-RecoilRate * UKismetMathLibrary::RandomFloatInRange(0.8f, 1.2f));


	if (!!CameraShakeClass)
	{
		APlayerController* controller = Character->GetController<APlayerController>();
		if (!!controller)
			controller->PlayerCameraManager->StartCameraShake(CameraShakeClass);
	}


	if (CurrSpreadRadius <= 1.0f)
	{
		CurrSpreadRadius += SpreadSpeed * GetWorld()->GetDeltaSeconds();

		CrossHair->UpdateSpreadRange(CurrSpreadRadius, MaxSpreadAlignment);
	}
	LastAddSpreadTime = GetWorld()->GetTimeSeconds();


	CurrMagazineCount--;

	if (CurrMagazineCount <= 0)
		Reload();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ DoAction ���� �Լ�
//// TODO: Ÿ�̸� �Ŵ����� ���� Ÿ�̸� ����, ��� Idle�� ����
void ACWeapon_Rifle::End_Fire()
{
	if (State->IsActionMode())
		State->SetIdleMode();

	if (bAutoFire)
		GetWorld()->GetTimerManager().ClearTimer(AutoFireHandle);

	bFiring = false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ������ ���� ���� üũ �Լ�
//// TODO: ����������, �̹� ������������ üũ�ϰ� ��� �ƴϸ� true�� ��ȯ
bool ACWeapon_Rifle::CanReload()
{
	bool b = false;

	b |= bEquipping;
	b |= bReload;

	return !b;

}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ������ �Լ�
//// TODO: ��� �ൿ�� �����ϰ� ������ �ִϸ��̼��� �����ϸ� ������
void ACWeapon_Rifle::Reload()
{
	bReload = true;

	End_SubAction();
	End_Fire();
	End_DoAction();

	State->SetReloadMode();

	if (!!ReloadMontage)
		Character->PlayAnimMontage(ReloadMontage);

}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ������ �Լ� �ִϸ��̼� ��Ƽ���� �Լ�
//// TODO: źâ �и�
void ACWeapon_Rifle::Eject_Magazine()
{
	if (MagazineBoneName.IsValid())
		Mesh->HideBoneByName(MagazineBoneName, PBO_None);


	CheckNull(MagazineClass);

	FTransform transform = Mesh->GetSocketTransform(MagazineBoneName);
	ACMagazine* magazine = GetWorld()->SpawnActorDeferred<ACMagazine>(MagazineClass, transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	magazine->Eject();
	magazine->SetLifeSpan(5);
	magazine->FinishSpawning(transform);

}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ������ �Լ� �ִϸ��̼� ��Ƽ���� �Լ�
//// TODO: �ɸ��Ͱ� �� �ڿ��� źâ�� ���� �ִϸ��̼� ������ ���� �� �� ���Ͽ� źâ ����
void ACWeapon_Rifle::Spawn_Magazine()
{
	CheckNull(MagazineClass);

	Magazine = GetWorld()->SpawnActor<ACMagazine>(MagazineClass);
	Magazine->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), MagazineAttachSocketName);

}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ������ �Լ� �ִϸ��̼� ��Ƽ���� �Լ�
//// TODO: �ѿ� źâ�� ���� �� ź�� ���� ������Ű�� �տ� �ִ� źâ�� �����ϰ� ����� �� �޽��� źâ �κ��� �ٽ� ���̰� ��
void ACWeapon_Rifle::Load_Magazine()
{
	if (!!Magazine)
		Magazine->Destroy();

	if (MagazineBoneName.IsValid())
		Mesh->UnHideBoneByName(MagazineBoneName);

	CurrMagazineCount = MaxMagazineCount;

}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ������ �Լ� �ִϸ��̼� ��Ƽ���� �Լ�
//// TODO: �ٽ� ����� �� �ֵ��� ��带 �ʱ�ȭ����
void ACWeapon_Rifle::End_Reload()
{
	bReload = false;
	State->SetIdleMode();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ���� Toggle �Լ�
//// TODO: ��Ŭ���� ��� ������ ������ ���簡 �ǵ��� ���� ���� ���� ����
void ACWeapon_Rifle::ToggleAutoFire()
{
	if (bAutoFire && bFiring)
		End_Fire();

	bAutoFire = !bAutoFire;

}
///////////////////////////////////////////////////////////////////////////////////////////////////


/* =========================================== PUBLIC =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* =========================================== PRIVATE =========================================== */

///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ DoAction �Լ�
//// TODO: Action���� ����, �������̰ų� ����� �� ���� ���� return. begin_fire�� ȣ���ϰ� �������� ���� �� �Լ��� Ÿ�̸ӿ� �����
void ACWeapon_Rifle::DoAction()
{
	if (CanFire() == false)
		return;

	if (State->IsJumpMode() == true)
		return;

	if (State->IsIdleMode() || State->IsActionMode())
	{
		State->SetActionMode();
		bFiring = true;

		if (bAutoFire)
		{
			GetWorld()->GetTimerManager().SetTimer(AutoFireHandle, this, &ACWeapon_Rifle::Begin_Fire, AutoFireInterval, true, 0);

			return;
		}

		Begin_Fire();
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ���� ���� ���� üũ �Լ�
//// TODO: ������, ��������, ���� ���� ������ üũ�� ��� �ƴϸ� true�� ��ȯ
bool ACWeapon_Rifle::CanAim()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bInAim;

	return !b;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ SubAction ��Ƽ���� �Լ�
//// TODO: ���� ������ �� üũ�ϰ� �����ϴٸ� �������� ���� AimData ����
void ACWeapon_Rifle::Begin_SubAction()
{
	if (CanAim() == false)
		return;

	bInAim = true;

	SpringArm->bEnableCameraLag = false;

	if (!!AimCurve)
	{
		Timeline->PlayFromStart();
		AimData.SetData(Character);

		return;
	}

	AimData.SetDataByNoneCurve(Character);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ SubAction ��Ƽ���� �Լ�
//// TODO: �����·� �����ϱ� ���� ������ AimData ����
void ACWeapon_Rifle::End_SubAction()
{
	CheckFalse(bInAim);
	bInAim = false;

	SpringArm->bEnableCameraLag = true;

	if (!!AimCurve)
	{
		Timeline->ReverseFromEnd();
		BaseData.SetData(Character);

		return;
	}

	BaseData.SetDataByNoneCurve(Character);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ���� �Լ�
//// @ param 1: Ÿ�Ӷ����� ��� ��
//// TODO: Lerp�Լ��� �̿��� Ÿ�Ӷ����� ������ FOV ����
void ACWeapon_Rifle::OnAiming(float Output)
{
	Camera->FieldOfView = FMath::Lerp<float>(AimData.FieldOfView, BaseData.FieldOfView, Output);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Q ��ų ��Ƽ���� �Լ�
//// TODO: �ڵ�����ϴ� ����ü�� Q ��ų Ŭ������ ������. �� �� �ɸ����� ��� �κп� �����ϸ� �ɸ��Ϳ� ȸ������ ����
void ACWeapon_Rifle::Begin_SkillQ()
{
	Super::Begin_SkillQ();

	CheckNull(SkillQClass);

	FActorSpawnParameters params;
	params.Owner = Character;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform transform;
	transform.SetRotation(FQuat(Character->GetActorRotation()));

	ACSkill1Rifle* skill;
	skill = Character->GetWorld()->SpawnActor<ACSkill1Rifle>(SkillQClass, transform, params);
	
	skill->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "Rifle_SkillQ_Left");
}
///////////////////////////////////////////////////////////////////////////////////////////////////


/* =========================================== PRIVATE =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
