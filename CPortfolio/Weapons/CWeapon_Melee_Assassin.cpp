#include "Weapons/CWeapon_Melee_Assassin.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CWeaponComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Widgets/CUserWidget_Target.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameframework/Character.h"
#include "CSkill2Assassin.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 생성자
ACWeapon_Melee_Assassin::ACWeapon_Melee_Assassin()
{
	CHelpers::GetAsset<UAnimMontage>(&EquipMontage, L"AnimMontage'/Game/GhostLady_S1/Montages/Assassin/Assassin_Equip_Montage.Assassin_Equip_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&UnEquipMontage, L"AnimMontage'/Game/GhostLady_S1/Montages/Assassin/Assassin_Unequip_Montage.Assassin_Unequip_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&SkillQMontage, L"AnimMontage'/Game/GhostLady_S1/Montages/Assassin/Assassin_SkillQ_Montage.Assassin_SkillQ_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&SkillEMontage, L"AnimMontage'/Game/GhostLady_S1/Montages/Assassin/Assassin_SkillE_Montage.Assassin_SkillE_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&DashMontage, L"AnimMontage'/Game/GhostLady_S1/Montages/Common/Dash_Assassin_Montage.Dash_Assassin_Montage'");

	CHelpers::GetAsset<UDataTable>(&DoActionDataTable, L"DataTable'/Game/BP/Weapons/Assassin/Assassin_DoAction.Assassin_DoAction'");
	CHelpers::GetAsset<UDataTable>(&HitDataTable, L"DataTable'/Game/BP/Weapons/Assassin/Assassin_HitData.Assassin_HitData'");
	CHelpers::GetAsset<UDataTable>(&AirComboDataTable, L"DataTable'/Game/BP/Weapons/Assassin/Assassin_AirCombo.Assassin_AirCombo'");
	CHelpers::GetAsset<UDataTable>(&AirHitDataTable, L"DataTable'/Game/BP/Weapons/Assassin/Assassin_AirHitData.Assassin_AirHitData'");
	CHelpers::GetAsset<UDataTable>(&SkillQHitDataTable, L"DataTable'/Game/BP/Weapons/Assassin/Assassin_SkillQHitData.Assassin_SkillQHitData'");
	CHelpers::GetAsset<UDataTable>(&SkillEHitDataTable, L"DataTable'/Game/BP/Weapons/Assassin/Assassin_SkillEHitData.Assassin_SkillEHitData'");

	CHelpers::GetClass<UCUserWidget_Target>(&TargetWidgetClass, "WidgetBlueprint'/Game/BP/Widgets/WB_Target.WB_Target_C'");

	bUseControlRotation = false;

	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Sword, "Sword", RootComponent);
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Collision, "Collision", Sword);

	UStaticMesh* mesh;
	CHelpers::GetAsset<UStaticMesh>(&mesh, L"StaticMesh'/Game/WeaponMesh/Assassin/Assassin.Assassin'");
	Sword->SetStaticMesh(mesh);
	Sword->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Collision->SetRelativeLocation(FVector(0, 30.0f, 0));
	Collision->SetRelativeRotation(FRotator(0, 0, -90));
	Collision->SetCapsuleHalfHeight(85.0f);
	CollisionRadius = 15.0f;
	Collision->SetCapsuleRadius(CollisionRadius);

	bCanMove = false;

	HolsterSocketName = "Assassin_Holster";
	EquipSocketName = "Assassin_RightHand";

	CHelpers::GetAsset<UNiagaraSystem>(&NGDashEffect, L"NiagaraSystem'/Game/Effects/NG_Assassin_AirCombo_Effect.NG_Assassin_AirCombo_Effect'");
	CHelpers::GetClass(&GhostTrailClass, "Blueprint'/Game/BP/Effect/BP_CGhostTrail.BP_CGhostTrail_C'");

	CHelpers::GetClass(&SkillEClass, "Blueprint'/Game/BP/Weapons/Assassin/BP_CSkill2Assassin.BP_CSkill2Assassin_C'");

	DashDistance = 15.0f;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Begin Play
//// TODO: 무기 메시를 케릭터 소켓에 붙임, UI등록
void ACWeapon_Melee_Assassin::BeginPlay()
{
	Super::BeginPlay();
	Sword->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, HolsterSocketName);

	if (!!TargetWidgetClass)
	{
		TargetWidget = CreateWidget<UCUserWidget_Target, APlayerController>(Character->GetController<APlayerController>(), TargetWidgetClass);
		TargetWidget->AddToViewport();
		TargetWidget->HiddenTarget();
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Tick
//// TODO: 스프링암 조절로 카메라 워킹, Q 스킬 모드일 때 타게팅 Trace
void ACWeapon_Melee_Assassin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// @ 카메라 워킹
	if (Weapon->IsAssassinMode() && !!springArm)
	{
		if (bInAir == true)
		{
			// 공중 콤보 중일 때
			springArm->TargetArmLength = UKismetMathLibrary::FInterpTo(springArm->TargetArmLength, InitSpringArmLength * 1.5, DeltaTime, 1.5f);
		}
		else if (bSkillQ == true)
		{
			// Q스킬 타게팅 액터를 잡을 때
			springArm->TargetArmLength = UKismetMathLibrary::FInterpTo(springArm->TargetArmLength, InitSpringArmLength / 3, DeltaTime, 2.0f);
			float targetZ = springArm->TargetOffset.Z;
			springArm->TargetOffset = FVector(0, 0, UKismetMathLibrary::FInterpTo(targetZ, -50, DeltaTime, 2.0f));
			float currFOV = camera->FieldOfView;
			camera->FieldOfView = UKismetMathLibrary::FInterpTo(currFOV, 60, DeltaTime, 1.0f);
		}
		else					
		{
			// 복귀
			springArm->TargetArmLength = UKismetMathLibrary::FInterpTo(springArm->TargetArmLength, InitSpringArmLength, DeltaTime, 1.5f);
			float targetZ = springArm->TargetOffset.Z;
			springArm->TargetOffset = FVector(0, 0, UKismetMathLibrary::FInterpTo(targetZ, 0, DeltaTime, 1.5f));
			float currFOV = camera->FieldOfView;
			camera->FieldOfView = UKismetMathLibrary::FInterpTo(currFOV, 90, DeltaTime, 1.0f);
		}
	}	// if(Weapon->IsAssassinMode() && !!springArm)

	// @ Q 스킬 타게팅 액터 잡기
	// TODO: BoxTrace를 이용해 타게팅을 하되, 화면 중앙에서 가장 가까운 적을 우선으로 하고 벽에 가려진 케릭터는 타게팅을 잡지 않음
	if (Weapon->IsAssassinMode() && State->IsSkillQMode())
	{
		// BoxTraceMultiObject를 이용해 가장 가까우며 화면의 중앙에서 가장 가까운 적을 타게팅
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
		TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
		TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
		objectTypes.Add(WorldStatic);
		objectTypes.Add(Pawn);
		TArray<FHitResult> hitResults;
		TArray<AActor*> ignores;
		ignores.Add(Character);
		UKismetSystemLibrary::BoxTraceMultiForObjects(
			GetWorld(),
			camera->GetComponentLocation() + (camera->GetForwardVector() * 200.0f),
			camera->GetComponentLocation() + (camera->GetForwardVector() * 2000.0f),
			FVector(0, 50.0f, 50.0f),
			camera->GetComponentRotation(),
			objectTypes,
			false,
			ignores,
			EDrawDebugTrace::None,
			hitResults,
			true
		);	// 거리 2000. 박스의 크기는 가로 50, 세로 50

		TargetEnemy = nullptr;				// 충돌된 적 케릭터가 없으면 Target은 null
		float angle = -2.0f;
		float detectDistance = 2000.0f;		// 감지된 가장 가까운 거리
		AActor* closestActor = nullptr;		// 가장 가깝게 충돌된 액터
		for (FHitResult hitResult : hitResults)
		{
			if (hitResult.bBlockingHit)
			{
				// 가장 가까운 적 케릭터를 계산 (임팩트된 지점으로부터 거리 구함)
				FVector len = Character->GetActorLocation() - hitResult.ImpactPoint;
				AActor* actor = Cast<AActor>(hitResult.GetActor());
				float distance = len.Size();
				if (!!actor && detectDistance > distance)
				{
					detectDistance = distance;
					closestActor = actor;
				}

				// 카메라 중앙에서 가장 가까운 적을 타게팅
				ACharacter* enemy = Cast<ACharacter>(actor);
				if (!!enemy)
				{
					if (attackedTarget.Find(enemy) >= 0)		// 이미 공격 당했던 적은 패스
						continue;

					FVector direction = enemy->GetActorLocation() - camera->GetComponentLocation();
					direction = direction.GetSafeNormal2D();

					FVector forward = FQuat(camera->GetComponentRotation()).GetForwardVector();

					// 내적 이용
					float dot = FVector::DotProduct(direction, forward);
					if (dot >= angle)
					{
						angle = dot;
						TargetEnemy = enemy;
					}
				}
			}
		}

		// 제일 가까운 액터가 적 케릭터가 아닌 경우 타게팅 끊어줌
		ACharacter* enemy = Cast<ACharacter>(closestActor);
		if (enemy == nullptr)
			TargetEnemy = nullptr;

		// 카메라에서 직선상으로 LineTraceChannel을 이용해 닿는 케릭터를 최우선순위로 여김
		FHitResult hit;
		UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			camera->GetComponentLocation() + (camera->GetForwardVector() * 200.0f),
			camera->GetComponentLocation() + (camera->GetForwardVector() * 2000.0f),
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			ignores,
			EDrawDebugTrace::None,
			hit,
			true
		);
		if (hit.bBlockingHit)
		{
			ACharacter* character = Cast<ACharacter>(hit.GetActor());
			if (!!character && attackedTarget.Find(character) < 0)
			{
				TargetEnemy = character;
			}
		}

		if (!!TargetEnemy)
			TargetWidget->VisibleTarget();
		else
			TargetWidget->HiddenTarget();
	}	// if (Weapon->IsAssassinMode() && State->IsSkillMode())
}
///////////////////////////////////////////////////////////////////////////////////////////////////


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* =========================================== PRIVATE =========================================== */

///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 장착 노티파이 함수
//// TODO: 칼 메시를 케릭터 손 위치의 소켓에 붙임
void ACWeapon_Melee_Assassin::Begin_Equip()
{
	Super::Begin_Equip();

	Sword->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, EquipSocketName);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 해제 노티파이 함수
//// TODO: 칼 메시를 케릭터의 허리 위치의 소켓에 붙임
void ACWeapon_Melee_Assassin::End_UnEquip()
{
	Super::End_UnEquip();

	Sword->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, L"Assassin_Holster");
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Skill Q (Input Q Key)
//// TODO: ControlRotationYaw 조작
void ACWeapon_Melee_Assassin::SkillQ()
{
	Super::SkillQ();

	Status->EnableControlRotation();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Q 스킬 노티파이 함수
//// TODO: 모드 변경, 스프링 암 프로퍼티 값 조작, 월드 딜레이 설정(슬로우 모션)
void ACWeapon_Melee_Assassin::Begin_SkillQ()
{
	Super::Begin_SkillQ();

	bSkillQ = true;
	State->SetSkillQMode();
	springArm->CameraLagSpeed = 7.0f;

	if(Qidx == 0)
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5f);
	else
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.7f);
}
void ACWeapon_Melee_Assassin::End_SkillQ()
{
	Super::End_SkillQ();
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	Status->DisableControlRotation();
	bSkillQ = false;
	Qidx = 0;
	TargetWidget->HiddenTarget();
	attackedTarget.Empty();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Skill E (Input E Key)
//// TODO: SkillE Assassin 클래스 스폰
void ACWeapon_Melee_Assassin::SkillE()
{
	Super::SkillE();

	Status->Stop();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ E 스킬 노티파이 함수
void ACWeapon_Melee_Assassin::Begin_SkillE()
{
	Super::Begin_SkillE();

	CheckNull(SkillEClass);

	FVector loc = Character->GetActorLocation() - (Character->GetActorForwardVector() * 1000) + FVector(0, 0, 1000);
	FRotator rot = Character->GetActorRotation();
	FTransform transform;
	transform.SetLocation(loc);
	transform.SetRotation(FQuat(rot));

	FActorSpawnParameters params;
	params.Owner = Character;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACSkill2Assassin* skill;
	skill = Character->GetWorld()->SpawnActor<ACSkill2Assassin>(SkillEClass, transform, params);
}

void ACWeapon_Melee_Assassin::End_SkillE()
{
	Super::End_SkillE();
	Status->Move();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 백어택 함수
//// TODO: 공중 콤보 시 공격 직전에 적의 등 뒤로 이동
void ACWeapon_Melee_Assassin::BackAttack()
{
	Super::BackAttack();

	if (bInAir == true && !!Candidate)
	{
		FVector direction = Candidate->GetActorLocation() - Character->GetActorLocation();
		direction = direction.GetSafeNormal2D();

		Character->SetActorLocation(Candidate->GetActorLocation() + direction * 100);
		FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), Candidate->GetActorLocation());
		Character->SetActorRotation(lookAt);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ SubSkill (Input X Key)
//// TODO: 텔레포트 가능한 위치인지 체크 후 텔레포트. 애니메이션 배열의 순서대로 애니메이션 적용 및 나이아가라 효과 적용
void ACWeapon_Melee_Assassin::SubSkill()
{
	Super::SubSkill();

	Status->DisableControlRotation();

	if (!!TargetEnemy)
	{
		TargetWidget->HiddenTarget();
		attackedTarget.AddUnique(TargetEnemy);

		// 텔레포트 시 Lag 속도를 낮춰 카메라가 뒤늦게 따라오도록 함
		springArm->CameraLagSpeed = 2.0f;

		FVector teleportLocation = TraceCanTeleportLocation(TargetEnemy);
		if (teleportLocation != DisableTeleportingLocation && Qidx < 5)
		{
			// 이동하기 전 자리에 잔상 효과
			CheckNull(NGDashEffect);
			UNiagaraFunctionLibrary::SpawnSystemAttached(NGDashEffect, Character->GetMesh(), "None", FVector(0, 0, 0), FRotator(0, 0, 0),
				EAttachLocation::KeepRelativeOffset, true, true, ENCPoolMethod::None, true);

			bSkillQ = false;
			State->SetSubSkillMode();

			// 이동
			{	
				FVector direction = TargetEnemy->GetActorLocation() - Character->GetActorLocation();
				direction = direction.GetSafeNormal2D();

				// 서로가 바라보도록 함
				Character->SetActorLocation(teleportLocation);
				FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), TargetEnemy->GetActorLocation());
				Character->SetActorRotation(lookAt);
				lookAt = UKismetMathLibrary::FindLookAtRotation(TargetEnemy->GetActorLocation(), Character->GetActorLocation());
				TargetEnemy->SetActorRotation(lookAt);
			}// 이동

			// 애니메이션
			Character->PlayAnimMontage(SkillQAttackMontages[Qidx]);
			TargetEnemy->PlayAnimMontage(SkillQHitMontages[Qidx]);
			Qidx++;

			// 데미지 처리
			UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(TargetEnemy);
			if (weapon)
				weapon->Damaged(Character, this, &SkillQHitData);
			
			// 이동한 후 자리에 잔상 효과
			UNiagaraFunctionLibrary::SpawnSystemAttached(NGDashEffect, Character->GetMesh(), "None", FVector(0, 0, 0), FRotator(0, 0, 0),
				EAttachLocation::KeepRelativeOffset, true, true, ENCPoolMethod::None, true);

		}	//if (teleportLocation != DisableTeleportingLocation && Qidx < 5)
	}	// if (!!TargetEnemy)
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 텔레포트 가능한 위치 탐색 함수
//// @ param 1: 타게팅된 케릭터
//// @ return : 텔레포트 가능한 위치
//// TODO: 케릭터의 전방부터 45도씩 8방향에 장애물이 있는지 확인
//// TODO: 장애물이 없다면 바닥 체크 함수를 호출해 케릭터의 발판이 있는 공간인지 확인
FVector ACWeapon_Melee_Assassin::TraceCanTeleportLocation(ACharacter* target)
{
	TArray<AActor*> ignores;
	ignores.Add(Character);
	FHitResult hitResult;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0) continue;

			FVector forwardVector = target->GetActorForwardVector() * i;
			FVector rightVector = target->GetActorRightVector() * j;
			FVector direction = forwardVector + rightVector;

			FVector capsuleLocation = (direction.GetSafeNormal2D() * distanceToEnemy[Qidx]) + target->GetActorLocation() + FVector(0, 0, 15.0f);
			UKismetSystemLibrary::CapsuleTraceSingle(
				GetWorld(),
				capsuleLocation,
				capsuleLocation,
				42.0f,
				96.0f,
				ETraceTypeQuery::TraceTypeQuery1,
				false,
				ignores,
				EDrawDebugTrace::None,
				hitResult,
				true
			);

			// false일 때, 바닥 체크
			if (hitResult.bBlockingHit == false)	
			{
				if (TraceBeingFloor(capsuleLocation) == true)
					return capsuleLocation;
			}
		}	// for j
	}	// for i

	return DisableTeleportingLocation;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 텔레포트 가능한 위치인지 바닥을 탐색하는 함수
//// @ param 1: 텔레포트 위치
//// @ return : Trace 충돌 여부
//// TODO: 케릭터가 발을 디딜 수 있는 공간이 있는지 체크
bool ACWeapon_Melee_Assassin::TraceBeingFloor(FVector capsuleLocation)
{
	// TODO: 텔레포트 가능한 캡슐 위치에서 바닥이 있는지 반환
	TArray<AActor*> ignores;
	ignores.Add(Character);
	FHitResult hitResult;
	FVector start = capsuleLocation - FVector(0, 0, 96.0f);
	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		start,
		start - FVector(0, 0, 30.0f),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ignores,
		EDrawDebugTrace::None,
		hitResult,
		true
	);

	return hitResult.bBlockingHit;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


/* =========================================== PRIVATE =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//

