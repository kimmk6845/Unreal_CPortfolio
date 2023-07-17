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
//// @ ������
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
//// TODO: ���� �޽ø� �ɸ��� ���Ͽ� ����, UI���
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
//// TODO: �������� ������ ī�޶� ��ŷ, Q ��ų ����� �� Ÿ���� Trace
void ACWeapon_Melee_Assassin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// @ ī�޶� ��ŷ
	if (Weapon->IsAssassinMode() && !!springArm)
	{
		if (bInAir == true)
		{
			// ���� �޺� ���� ��
			springArm->TargetArmLength = UKismetMathLibrary::FInterpTo(springArm->TargetArmLength, InitSpringArmLength * 1.5, DeltaTime, 1.5f);
		}
		else if (bSkillQ == true)
		{
			// Q��ų Ÿ���� ���͸� ���� ��
			springArm->TargetArmLength = UKismetMathLibrary::FInterpTo(springArm->TargetArmLength, InitSpringArmLength / 3, DeltaTime, 2.0f);
			float targetZ = springArm->TargetOffset.Z;
			springArm->TargetOffset = FVector(0, 0, UKismetMathLibrary::FInterpTo(targetZ, -50, DeltaTime, 2.0f));
			float currFOV = camera->FieldOfView;
			camera->FieldOfView = UKismetMathLibrary::FInterpTo(currFOV, 60, DeltaTime, 1.0f);
		}
		else					
		{
			// ����
			springArm->TargetArmLength = UKismetMathLibrary::FInterpTo(springArm->TargetArmLength, InitSpringArmLength, DeltaTime, 1.5f);
			float targetZ = springArm->TargetOffset.Z;
			springArm->TargetOffset = FVector(0, 0, UKismetMathLibrary::FInterpTo(targetZ, 0, DeltaTime, 1.5f));
			float currFOV = camera->FieldOfView;
			camera->FieldOfView = UKismetMathLibrary::FInterpTo(currFOV, 90, DeltaTime, 1.0f);
		}
	}	// if(Weapon->IsAssassinMode() && !!springArm)

	// @ Q ��ų Ÿ���� ���� ���
	// TODO: BoxTrace�� �̿��� Ÿ������ �ϵ�, ȭ�� �߾ӿ��� ���� ����� ���� �켱���� �ϰ� ���� ������ �ɸ��ʹ� Ÿ������ ���� ����
	if (Weapon->IsAssassinMode() && State->IsSkillQMode())
	{
		// BoxTraceMultiObject�� �̿��� ���� ������ ȭ���� �߾ӿ��� ���� ����� ���� Ÿ����
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
		);	// �Ÿ� 2000. �ڽ��� ũ��� ���� 50, ���� 50

		TargetEnemy = nullptr;				// �浹�� �� �ɸ��Ͱ� ������ Target�� null
		float angle = -2.0f;
		float detectDistance = 2000.0f;		// ������ ���� ����� �Ÿ�
		AActor* closestActor = nullptr;		// ���� ������ �浹�� ����
		for (FHitResult hitResult : hitResults)
		{
			if (hitResult.bBlockingHit)
			{
				// ���� ����� �� �ɸ��͸� ��� (����Ʈ�� �������κ��� �Ÿ� ����)
				FVector len = Character->GetActorLocation() - hitResult.ImpactPoint;
				AActor* actor = Cast<AActor>(hitResult.GetActor());
				float distance = len.Size();
				if (!!actor && detectDistance > distance)
				{
					detectDistance = distance;
					closestActor = actor;
				}

				// ī�޶� �߾ӿ��� ���� ����� ���� Ÿ����
				ACharacter* enemy = Cast<ACharacter>(actor);
				if (!!enemy)
				{
					if (attackedTarget.Find(enemy) >= 0)		// �̹� ���� ���ߴ� ���� �н�
						continue;

					FVector direction = enemy->GetActorLocation() - camera->GetComponentLocation();
					direction = direction.GetSafeNormal2D();

					FVector forward = FQuat(camera->GetComponentRotation()).GetForwardVector();

					// ���� �̿�
					float dot = FVector::DotProduct(direction, forward);
					if (dot >= angle)
					{
						angle = dot;
						TargetEnemy = enemy;
					}
				}
			}
		}

		// ���� ����� ���Ͱ� �� �ɸ��Ͱ� �ƴ� ��� Ÿ���� ������
		ACharacter* enemy = Cast<ACharacter>(closestActor);
		if (enemy == nullptr)
			TargetEnemy = nullptr;

		// ī�޶󿡼� ���������� LineTraceChannel�� �̿��� ��� �ɸ��͸� �ֿ켱������ ����
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
//// @ ���� ��Ƽ���� �Լ�
//// TODO: Į �޽ø� �ɸ��� �� ��ġ�� ���Ͽ� ����
void ACWeapon_Melee_Assassin::Begin_Equip()
{
	Super::Begin_Equip();

	Sword->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, EquipSocketName);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ���� ��Ƽ���� �Լ�
//// TODO: Į �޽ø� �ɸ����� �㸮 ��ġ�� ���Ͽ� ����
void ACWeapon_Melee_Assassin::End_UnEquip()
{
	Super::End_UnEquip();

	Sword->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, L"Assassin_Holster");
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Skill Q (Input Q Key)
//// TODO: ControlRotationYaw ����
void ACWeapon_Melee_Assassin::SkillQ()
{
	Super::SkillQ();

	Status->EnableControlRotation();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Q ��ų ��Ƽ���� �Լ�
//// TODO: ��� ����, ������ �� ������Ƽ �� ����, ���� ������ ����(���ο� ���)
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
//// TODO: SkillE Assassin Ŭ���� ����
void ACWeapon_Melee_Assassin::SkillE()
{
	Super::SkillE();

	Status->Stop();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ E ��ų ��Ƽ���� �Լ�
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
//// @ ����� �Լ�
//// TODO: ���� �޺� �� ���� ������ ���� �� �ڷ� �̵�
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
//// TODO: �ڷ���Ʈ ������ ��ġ���� üũ �� �ڷ���Ʈ. �ִϸ��̼� �迭�� ������� �ִϸ��̼� ���� �� ���̾ư��� ȿ�� ����
void ACWeapon_Melee_Assassin::SubSkill()
{
	Super::SubSkill();

	Status->DisableControlRotation();

	if (!!TargetEnemy)
	{
		TargetWidget->HiddenTarget();
		attackedTarget.AddUnique(TargetEnemy);

		// �ڷ���Ʈ �� Lag �ӵ��� ���� ī�޶� �ڴʰ� ��������� ��
		springArm->CameraLagSpeed = 2.0f;

		FVector teleportLocation = TraceCanTeleportLocation(TargetEnemy);
		if (teleportLocation != DisableTeleportingLocation && Qidx < 5)
		{
			// �̵��ϱ� �� �ڸ��� �ܻ� ȿ��
			CheckNull(NGDashEffect);
			UNiagaraFunctionLibrary::SpawnSystemAttached(NGDashEffect, Character->GetMesh(), "None", FVector(0, 0, 0), FRotator(0, 0, 0),
				EAttachLocation::KeepRelativeOffset, true, true, ENCPoolMethod::None, true);

			bSkillQ = false;
			State->SetSubSkillMode();

			// �̵�
			{	
				FVector direction = TargetEnemy->GetActorLocation() - Character->GetActorLocation();
				direction = direction.GetSafeNormal2D();

				// ���ΰ� �ٶ󺸵��� ��
				Character->SetActorLocation(teleportLocation);
				FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), TargetEnemy->GetActorLocation());
				Character->SetActorRotation(lookAt);
				lookAt = UKismetMathLibrary::FindLookAtRotation(TargetEnemy->GetActorLocation(), Character->GetActorLocation());
				TargetEnemy->SetActorRotation(lookAt);
			}// �̵�

			// �ִϸ��̼�
			Character->PlayAnimMontage(SkillQAttackMontages[Qidx]);
			TargetEnemy->PlayAnimMontage(SkillQHitMontages[Qidx]);
			Qidx++;

			// ������ ó��
			UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(TargetEnemy);
			if (weapon)
				weapon->Damaged(Character, this, &SkillQHitData);
			
			// �̵��� �� �ڸ��� �ܻ� ȿ��
			UNiagaraFunctionLibrary::SpawnSystemAttached(NGDashEffect, Character->GetMesh(), "None", FVector(0, 0, 0), FRotator(0, 0, 0),
				EAttachLocation::KeepRelativeOffset, true, true, ENCPoolMethod::None, true);

		}	//if (teleportLocation != DisableTeleportingLocation && Qidx < 5)
	}	// if (!!TargetEnemy)
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ �ڷ���Ʈ ������ ��ġ Ž�� �Լ�
//// @ param 1: Ÿ���õ� �ɸ���
//// @ return : �ڷ���Ʈ ������ ��ġ
//// TODO: �ɸ����� ������� 45���� 8���⿡ ��ֹ��� �ִ��� Ȯ��
//// TODO: ��ֹ��� ���ٸ� �ٴ� üũ �Լ��� ȣ���� �ɸ����� ������ �ִ� �������� Ȯ��
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

			// false�� ��, �ٴ� üũ
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
//// @ �ڷ���Ʈ ������ ��ġ���� �ٴ��� Ž���ϴ� �Լ�
//// @ param 1: �ڷ���Ʈ ��ġ
//// @ return : Trace �浹 ����
//// TODO: �ɸ��Ͱ� ���� ��� �� �ִ� ������ �ִ��� üũ
bool ACWeapon_Melee_Assassin::TraceBeingFloor(FVector capsuleLocation)
{
	// TODO: �ڷ���Ʈ ������ ĸ�� ��ġ���� �ٴ��� �ִ��� ��ȯ
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

