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
//// @ 커브 값이 있을 때 AimData 로드
//// TODO: 스프링암 길이 및 위치 조작
void FWeaponAimData::SetData(ACharacter* InOwner)
{
	USpringArmComponent* springArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	springArm->TargetArmLength = TargetArmLength;
	springArm->SocketOffset = SocketOffset;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 커브 값이 없을 때 AimData 로드
//// TODO: 스프링암 길이 및 위치 조작, 카메라 FOV 조작
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
//// @ 생성자
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
//// TODO: 타임라인에 조준 함수 등록, UI 로드, 총 메시를 케릭터 소켓에 붙임
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
//// TODO: 현재 시간과 마지막 사격 시간을 비교해 일정 시간이 지나면 반동, 탄퍼짐, UI 초기화
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
//// @ 장착 할 수 있는지 체크하는 함수
//// TODO: 장착중, 재장전, 사격 중인지 변수를 체크해 모두 아니면 true를 리턴
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
//// @ 장착 관련 노티파이 함수
//// TODO: 총 메시를 케릭터 손 위치의 소켓에 붙임
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
//// @ 장착 할 수 있는지 체크하는 함수
//// TODO: 장착 중인지, 재장전 중인지 확인하고 모두 아니면 true를 반환
bool ACWeapon_Rifle::CanUnequip()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;

	return !b;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 해제 관련 노티파이 함수
//// TODO: 조준 및 사격중이였다면, 이를 해제하게 만들고 총 메시들 케릭터의 등 뒤 소켓에 붙임. UI 숨김
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
//// @ 사격할 수 있는지 체크하는 함수
//// TODO: 장착중인지, 재장전중인지, 사격중인지 확인해 모두 아니면 true를 반환
bool ACWeapon_Rifle::CanFire()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;	// 연사를 위함

	return !b;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ DoAction 관련 함수
//// TODO: 한발의 총알이 날아가는 코드
//// TODO: 총알 클래스를 이용해 날아가는 총알을 만들고, 이는 적에게 아무 영향을 주지 않음
//// TODO: LineTraceChaanel을 이용해 실질적으로 타격할 총알을 구현. 타격된 위치에 효과 부여
//// TODO: 케릭터가 피격되면 피격 부위마다 데미지 차별화, 피격 효과 부여
void ACWeapon_Rifle::Begin_Fire()
{
	// 총알 클래스는 그냥 날아가게 보이는 효과. 실질적으로 타격을 입히는건 LineTrace를 이용함
	if (State->IsJumpMode() == true)
	{
		End_Fire();
		return;
	}

	State->SetActionMode();

	FVector direction = Camera->GetForwardVector();
	FTransform transform = Camera->GetComponentToWorld();

	FVector start = transform.GetLocation() + direction;
	
	// 탄 퍼짐
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
	//UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery6, false, ignores, EDrawDebugTrace::None, hitResult, true);		// Trace는 Pawn
	if (hitResult.bBlockingHit)
	{
		ACharacter* hitCharacter = Cast<ACharacter>(hitResult.GetActor());
		if (!!hitCharacter)		// 케릭터 피격
		{
			UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(hitCharacter);
			CheckNull(weapon);

			if (!!HitNiagara)
			{
				FRotator impactRotation = UKismetMathLibrary::FindLookAtRotation(hitResult.Location, hitResult.TraceStart);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitNiagara, hitResult.Location, impactRotation);
							
				/// 데미지 처리
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
		else	// 지형에 맞은 것
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

	// 반동
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
//// @ DoAction 관련 함수
//// TODO: 타이머 매니저를 통해 타이머 제거, 모드 Idle로 변경
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
//// @ 재장전 가능 여부 체크 함수
//// TODO: 장착중인지, 이미 재장전중인지 체크하고 모두 아니면 true를 반환
bool ACWeapon_Rifle::CanReload()
{
	bool b = false;

	b |= bEquipping;
	b |= bReload;

	return !b;

}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 재장전 함수
//// TODO: 모든 행동을 중지하고 재장전 애니메이션을 동작하며 재장전
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
//// @ 재장전 함수 애니메이션 노티파이 함수
//// TODO: 탄창 분리
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
//// @ 재장전 함수 애니메이션 노티파이 함수
//// TODO: 케릭터가 등 뒤에서 탄창을 빼는 애니메이션 동작을 취할 때 손 소켓에 탄창 부착
void ACWeapon_Rifle::Spawn_Magazine()
{
	CheckNull(MagazineClass);

	Magazine = GetWorld()->SpawnActor<ACMagazine>(MagazineClass);
	Magazine->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), MagazineAttachSocketName);

}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 재장전 함수 애니메이션 노티파이 함수
//// TODO: 총에 탄창을 넣을 때 탄알 수를 증가시키며 손에 있는 탄창을 제거하고 숨겼던 총 메시의 탄창 부분을 다시 보이게 함
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
//// @ 재장전 함수 애니메이션 노티파이 함수
//// TODO: 다시 사격할 수 있도록 모드를 초기화해줌
void ACWeapon_Rifle::End_Reload()
{
	bReload = false;
	State->SetIdleMode();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 연사 Toggle 함수
//// TODO: 좌클릭을 계속 누르고 있으면 연사가 되도록 연사 여부 변수 조작
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
//// @ DoAction 함수
//// TODO: Action모드로 변경, 점프중이거나 사격할 수 없을 때는 return. begin_fire를 호출하고 연사중일 때는 이 함수를 타이머에 등록함
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
//// @ 조준 가능 여부 체크 함수
//// TODO: 장착중, 재장전중, 조준 여부 변수를 체크해 모두 아니면 true를 반환
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
//// @ SubAction 노티파이 함수
//// TODO: 조준 가능한 지 체크하고 가능하다면 조준했을 때의 AimData 적용
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
//// @ SubAction 노티파이 함수
//// TODO: 원상태로 복귀하기 위해 기존의 AimData 적용
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
//// @ 조준 함수
//// @ param 1: 타임라인의 출력 값
//// TODO: Lerp함수를 이용해 타임라인의 값으로 FOV 보간
void ACWeapon_Rifle::OnAiming(float Output)
{
	Camera->FieldOfView = FMath::Lerp<float>(AimData.FieldOfView, BaseData.FieldOfView, Output);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Q 스킬 노티파이 함수
//// TODO: 자동사격하는 비행체인 Q 스킬 클래스를 스폰함. 이 때 케릭터의 어깨 부분에 스폰하며 케릭터와 회전값을 가짐
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
