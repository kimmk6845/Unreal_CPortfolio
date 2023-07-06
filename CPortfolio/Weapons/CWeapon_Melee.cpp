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
//// @ 생성자
ACWeapon_Melee::ACWeapon_Melee()
{
	PrimaryActorTick.bCanEverTick = true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Begin Play
//// TODO: 각 콜리전 컴포넌트에 Overlap 델리게이트 등록 및 콜리전 해제
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
//// @ 콜리전 범위 확대
//// TODO: 초기 콜리전 범위를 저장하고 범위를 넓혀 적용
void ACWeapon_Melee::OnRangeCollision()
{
	OriginCollisionRadius = CollisionRadius;
	CollisionRadius = CollisionRadius * 10;

	Collision->SetCapsuleRadius(CollisionRadius);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 콜리전 범위 축소
//// TODO: 초기 콜리전 범위로 축소시켜 적용
void ACWeapon_Melee::OffRangeCollision()
{
	CollisionRadius = OriginCollisionRadius;

	Collision->SetCapsuleRadius(CollisionRadius);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 콜리전 On 함수(노티파이)
//// TODO: 공격 바로 직전에 콜리전을 On시켜줌. 이 때 잔상 파티클도 보여줌
void ACWeapon_Melee::OnCollision()
{
	for (UShapeComponent* collision : Collisions)
		collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if (InAir == true)
	{
		// 잔상 효과
		CheckNull(GhostTrailClass);

		FActorSpawnParameters params;
		params.Owner = Character;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector vector = Character->GetActorLocation();
		vector.Z -= Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		FTransform transform;
		transform.SetTranslation(vector);

		GhostTrail = Character->GetWorld()->SpawnActor<ACGhostTrail>(GhostTrailClass, transform, params);


		// 나이아가라 Trail (Smoke)
		CheckNull(NGDashEffect);
		UNiagaraFunctionLibrary::SpawnSystemAttached(NGDashEffect, Character->GetMesh(), "None", FVector(0, 0, 0), FRotator(0, 0, 0),
			EAttachLocation::KeepRelativeOffset, true, true, ENCPoolMethod::None, true);

	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 콜리전 Off 함수(노티파이)
//// TODO: 공격 동작 후 콜리전을 해제해줌. 공격당한 적이 있다면 가장 정면에 있는 적을 바라보도록 함
void ACWeapon_Melee::OffColiision()
{
	for (UShapeComponent* collision : Collisions)
		collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 히트된 적들 중 가장 정면에 있던 적을 바라봄
	float angle = -2.0f;
	Candidate = nullptr;
	for (ACharacter* hitted : Hitted)
	{
		FVector direction = hitted->GetActorLocation() - Character->GetActorLocation();
		direction = direction.GetSafeNormal2D();

		FVector forward = FQuat(Character->GetControlRotation()).GetForwardVector();

		// 내적 이용
		float dot = FVector::DotProduct(direction, forward);
		if (dot >= angle)
		{
			angle = dot;
			Candidate = hitted;
		}
	}

	// 공콤 아닐 때만 정면 적 바라보기
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
//// @ 케릭터 Launch 함수
//// TODO: 공중 콤보를 자연스럽게 하기 위해 공격 때마다 Z값에 Launch를 해줌
void ACWeapon_Melee::LaunchCharacter()
{
	float launchZ = AirComboDatas[index].Launch;
	Character->LaunchCharacter(FVector(0, 0, launchZ), false, false);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 케릭터 Launch 함수
//// TODO: 마지막 공중 콤보 동작 때 케릭터를 Down시킴
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
//// @ DoAction (입력: 좌클릭)
//// TODO: 데이터 테이블의 공격 애니메이션을 출력하고 콤보 변수값 조작.
//// TODO: 케릭터가 공중콤보 시전 동작을 수행 후 DoAction이 호출되면, 공중 콤보 동작을 수행
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
				// 우클릭으로 공중 콤보 시전 동작을 하므로 index = 1부터 콤보 공격 시작
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
//// @ DoAction 노티파이 함수
//// TODO: Combo노티파이에 의해 콤보 변수가 true이면 인덱스 값을 늘려 다음 콤보 동작을 수행하도록 함
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
//// @ DoAction 노티파이 함수
//// TODO: 인덱스값 초기화, 공중 콤보 변수 초기화
void ACWeapon_Melee::End_DoAction()
{
	Super::End_DoAction();
	index = 0;
	InAir = false;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 랜덤 인덱스 DoAction 함수 (AI의 다양한 공격 모션을 위함)
//// TODO: 인덱스값 랜덤 부여. DoAction 공격
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
//// @ SubAction 노티파이 함수
//// TODO: 공중 콤보 시전 동작 수행, 모드 변경 및 공콤 변수 조작
void ACWeapon_Melee::Begin_SubAction()
{
	Super::Begin_SubAction();
	// 위로 올려치기 & 런치
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
//// @ 콜리전 overlap 함수
//// @ param 1: overlap된 케릭터
//// @ param 2: overlap된 케릭터의 콜리전 컴포넌트
//// TODO: 공격당한 케릭터들을 모아놓는 배열에 중복이 없이 등록하고 데미지를 전달

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
//// @ overlap 델리게이트 함수
//// @ param 1: 겹쳐진 충돌체의 primitive 컴포넌트
//// @ param 2: 겹쳐진 액터
//// @ param 3: 겹쳐진 액터의 primitive 컴포넌트
//// @ param 4: 겹쳐진 액터의 BodyIndex
//// @ param 5: 이동 중에 충돌 검사를 할 지 판단
//// @ param 6: 충돌 정보를 가진 FHitResult 구조체
//// TODO: OnBeginOverlap 함수 호출
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
