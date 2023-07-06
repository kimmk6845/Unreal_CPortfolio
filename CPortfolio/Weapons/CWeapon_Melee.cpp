#include "Weapons/CWeapon_Melee.h"
#include "Global.h"
#include "Components/ShapeComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CWeaponComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimMontage.h"
#include "AddOns/CGhostTrail.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ������
ACWeapon_Melee::ACWeapon_Melee()
{
	PrimaryActorTick.bCanEverTick = true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Begin Play
//// TODO: �� �ݸ��� ������Ʈ�� Overlap ��������Ʈ ��� �� �ݸ��� ����
void ACWeapon_Melee::BeginPlay()
{
	Super::BeginPlay();

	GetComponents<UShapeComponent>(Collisions);
	OffColiision();

	for (UShapeComponent* component : Collisions)
	{
		component->OnComponentBeginOverlap.AddDynamic(this, &ACWeapon_Melee::OnComponentBeginOverlap);
	}

	springArm = CHelpers::GetComponent<USpringArmComponent>(Character);
	if (!!springArm)
		InitSpringArmLength = springArm->TargetArmLength;
	camera = CHelpers::GetComponent<UCameraComponent>(Character);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Tick
void ACWeapon_Melee::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* =========================================== PUBLIC =========================================== */


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ �ݸ��� ���� Ȯ��
//// TODO: �ʱ� �ݸ��� ������ �����ϰ� ������ ���� ����
void ACWeapon_Melee::OnRangeCollision()
{
	OriginCollisionRadius = CollisionRadius;
	CollisionRadius = CollisionRadius * 10;

	Collision->SetCapsuleRadius(CollisionRadius);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ �ݸ��� ���� ���
//// TODO: �ʱ� �ݸ��� ������ ��ҽ��� ����
void ACWeapon_Melee::OffRangeCollision()
{
	CollisionRadius = OriginCollisionRadius;

	Collision->SetCapsuleRadius(CollisionRadius);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ �ݸ��� On �Լ�(��Ƽ����)
//// TODO: ���� �ٷ� ������ �ݸ����� On������. �� �� �ܻ� ��ƼŬ�� ������
void ACWeapon_Melee::OnCollision()
{
	for (UShapeComponent* collision : Collisions)
		collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if (InAir == true)
	{
		// �ܻ� ȿ��
		CheckNull(GhostTrailClass);

		FActorSpawnParameters params;
		params.Owner = Character;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector vector = Character->GetActorLocation();
		vector.Z -= Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		FTransform transform;
		transform.SetTranslation(vector);

		GhostTrail = Character->GetWorld()->SpawnActor<ACGhostTrail>(GhostTrailClass, transform, params);


		// ���̾ư��� Trail (Smoke)
		CheckNull(NGDashEffect);
		UNiagaraFunctionLibrary::SpawnSystemAttached(NGDashEffect, Character->GetMesh(), "None", FVector(0, 0, 0), FRotator(0, 0, 0),
			EAttachLocation::KeepRelativeOffset, true, true, ENCPoolMethod::None, true);

	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ �ݸ��� Off �Լ�(��Ƽ����)
//// TODO: ���� ���� �� �ݸ����� ��������. ���ݴ��� ���� �ִٸ� ���� ���鿡 �ִ� ���� �ٶ󺸵��� ��
void ACWeapon_Melee::OffColiision()
{
	for (UShapeComponent* collision : Collisions)
		collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ��Ʈ�� ���� �� ���� ���鿡 �ִ� ���� �ٶ�
	float angle = -2.0f;
	Candidate = nullptr;
	for (ACharacter* hitted : Hitted)
	{
		FVector direction = hitted->GetActorLocation() - Character->GetActorLocation();
		direction = direction.GetSafeNormal2D();

		FVector forward = FQuat(Character->GetControlRotation()).GetForwardVector();

		// ���� �̿�
		float dot = FVector::DotProduct(direction, forward);
		if (dot >= angle)
		{
			angle = dot;
			Candidate = hitted;
		}
	}

	// ���� �ƴ� ���� ���� �� �ٶ󺸱�
	if (InAir == false)
	{
		if (!!Candidate)
		{
			FRotator rotator = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), Candidate->GetActorLocation());
			FRotator originRotator = Character->GetActorRotation();
			Character->SetActorRotation(FRotator(originRotator.Pitch, rotator.Yaw, originRotator.Roll));
		}
	}

	BackAttack();

	if (!!GhostTrail)
		GhostTrail->Destroy();


	Hitted.Empty();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ �ɸ��� Launch �Լ�
//// TODO: ���� �޺��� �ڿ������� �ϱ� ���� ���� ������ Z���� Launch�� ����
void ACWeapon_Melee::LaunchCharacter()
{
	float launchZ = AirComboDatas[index].Launch;
	Character->LaunchCharacter(FVector(0, 0, launchZ), false, false);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ �ɸ��� Launch �Լ�
//// TODO: ������ ���� �޺� ���� �� �ɸ��͸� Down��Ŵ
void ACWeapon_Melee::DownCharacter()
{
	Character->LaunchCharacter(FVector(0, 0, -2000), false, false);

	for (ACharacter* hitted : Hitted)
	{
		hitted->LaunchCharacter(FVector(0, 0, -2000), false, false);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////


/* =========================================== PUBLIC =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* ========================================== PROTECTED ========================================== */


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ DoAction (�Է�: ��Ŭ��)
//// TODO: ������ ���̺��� ���� �ִϸ��̼��� ����ϰ� �޺� ������ ����.
//// TODO: �ɸ��Ͱ� �����޺� ���� ������ ���� �� DoAction�� ȣ��Ǹ�, ���� �޺� ������ ����
void ACWeapon_Melee::DoAction()
{
	Super::DoAction();

	if (InAir == true)
	{
		if (AirComboDatas.Num() > 0)
		{
			if (bEnable == true)
			{
				bEnable = false;
				bExist = true;
			}
			else
			{
				// ��Ŭ������ ���� �޺� ���� ������ �ϹǷ� index = 1���� �޺� ���� ����
				if (index == 0)
				{
					index++;
					PLAY_ANIM_AIRMONTAGE(index);	// index = 1
				}
			}
		}

		return;
	}
	else
	{
		if (DoActionDatas.Num() > 0)
		{
			if (bEnable == true)
			{
				bEnable = false;
				bExist = true;
			}
			else
			{
				if (State->IsIdleMode() == true)
				{
					State->SetActionMode();
					PLAY_ANIM_MONTAGE(0);			// index = 0
				}
			}
		}
	} // if (InAir == true)
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ DoAction ��Ƽ���� �Լ�
//// TODO: Combo��Ƽ���̿� ���� �޺� ������ true�̸� �ε��� ���� �÷� ���� �޺� ������ �����ϵ��� ��
void ACWeapon_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();

	if (bExist == true)
	{
		bExist = false;
		index++;

		if (InAir == true)
		{
			PLAY_ANIM_AIRMONTAGE(index);
			float launchZ = AirComboDatas[index].Launch;
			Character->LaunchCharacter(FVector(0, 0, launchZ), false, true);
		}
		else
		{
			PLAY_ANIM_MONTAGE(index);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ DoAction ��Ƽ���� �Լ�
//// TODO: �ε����� �ʱ�ȭ, ���� �޺� ���� �ʱ�ȭ
void ACWeapon_Melee::End_DoAction()
{
	Super::End_DoAction();
	index = 0;
	InAir = false;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ���� �ε��� DoAction �Լ� (AI�� �پ��� ���� ����� ����)
//// TODO: �ε����� ���� �ο�. DoAction ����
void ACWeapon_Melee::RandomDoAction()
{
	Super::RandomDoAction();
	
	index = UKismetMathLibrary::RandomIntegerInRange(0, DoActionDatas.Num() - 1);

	if (DoActionDatas.Num() > 0)
	{
		if (State->IsIdleMode() == true)
		{
			State->SetActionMode();
			PLAY_ANIM_MONTAGE(index);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ SubAction ��Ƽ���� �Լ�
//// TODO: ���� �޺� ���� ���� ����, ��� ���� �� ���� ���� ����
void ACWeapon_Melee::Begin_SubAction()
{
	Super::Begin_SubAction();
	// ���� �÷�ġ�� & ��ġ
	if (State->IsIdleMode() == true)
	{
		State->SetActionMode();
		InAir = true;

		PLAY_ANIM_AIRMONTAGE(0);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////


/* ========================================== PROTECTED ========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* =========================================== PRIVATE =========================================== */


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ �ݸ��� overlap �Լ�
//// @ param 1: overlap�� �ɸ���
//// @ param 2: overlap�� �ɸ����� �ݸ��� ������Ʈ
//// TODO: ���ݴ��� �ɸ��͵��� ��Ƴ��� �迭�� �ߺ��� ���� ����ϰ� �������� ����

void ACWeapon_Melee::OnBeginOverlap(ACharacter* otherCharacter, UShapeComponent* attackComponent)
{
	if (Hitted.Find(otherCharacter) < 0)
	{
		Hitted.AddUnique(otherCharacter);

		UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(otherCharacter);
		if (!!weapon)
		{
			if (InAir == true)
			{
				weapon->Damaged(Character, this, &AirHitDatas[index]);

				float launchZ = AirComboDatas[index].Launch;
				otherCharacter->LaunchCharacter(FVector(0, 0, launchZ), false, true);
			}
			else
			{
				weapon->Damaged(Character, this, &HitDatas[index]);
			}
		}
	} // if (Hitted.Find(otherCharacter) < 0)
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ overlap ��������Ʈ �Լ�
//// @ param 1: ������ �浹ü�� primitive ������Ʈ
//// @ param 2: ������ ����
//// @ param 3: ������ ������ primitive ������Ʈ
//// @ param 4: ������ ������ BodyIndex
//// @ param 5: �̵� �߿� �浹 �˻縦 �� �� �Ǵ�
//// @ param 6: �浹 ������ ���� FHitResult ����ü
//// TODO: OnBeginOverlap �Լ� ȣ��
void ACWeapon_Melee::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(Character == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	CheckNull(character);
	UShapeComponent* shapeComponent = Cast<UShapeComponent>(OtherComp);
	CheckNull(shapeComponent);

	OnBeginOverlap(character, shapeComponent);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


/* =========================================== PRIVATE =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
