#include "Weapons/CSkill2Assassin.h"
#include "Global.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

#include "Components/CWeaponComponent.h"
#include "CWeapon.h"

ACSkill2Assassin::ACSkill2Assassin()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;		// 초기 틱은 Off. 스폰위치에 활성화 되면 tick 활성화

	//// 컴포넌트 생성
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Cylinder, "Cylinder", Scene);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Cone, "Cone", Cylinder);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &BodyRed, "BodyRed", Cylinder);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &BodyRedLine1, "BodyRedLine1", Cylinder);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &BodyRedLine2, "BodyRedLine2", Cylinder);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &BodyRedTop, "BodyRedTop", Cylinder);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &BodyRedBottom, "BodyRedBottom", Cylinder);

	CHelpers::CreateComponent<USphereComponent>(this, &Collision, "Collision", Cone);

	CHelpers::CreateComponent<UNiagaraComponent>(this, &NiagaraComponent, "NiagaraComponent", Scene);
	CHelpers::CreateComponent<UAudioComponent>(this, &Sound, "Sound", Scene);
	//// 컴포넌트 생성 ///// End


	//// Location, Rotation, Scale 조정
	Cylinder->SetRelativeRotation(FRotator(-90, 180, 180));
	Cylinder->SetRelativeScale3D(FVector(0.025f, 0.025f, 0.6f));

	Cone->SetRelativeLocation(FVector(0, 0, 60.0f));
	Cone->SetRelativeScale3D(FVector(1.2f, 1.2f, 0.2f));

	BodyRed->SetRelativeLocation(FVector(0, 0, -22.3f));
	BodyRed->SetRelativeScale3D(FVector(1.1f, 1.1f, 0.2f));

	BodyRedLine1->SetRelativeLocation(FVector(0, 0, -18.24f));
	BodyRedLine1->SetRelativeScale3D(FVector(1.2f, 1.2f, 0.01f));

	BodyRedLine2->SetRelativeLocation(FVector(0, 0, -26.67f));
	BodyRedLine2->SetRelativeScale3D(FVector(1.2f, 1.2f, 0.01f));

	BodyRedTop->SetRelativeLocation(FVector(0, 0, 47.42f));
	BodyRedTop->SetRelativeScale3D(FVector(1.1f, 1.1f, 0.05f));

	BodyRedBottom->SetRelativeLocation(FVector(0, 0, -50.0f));
	BodyRedBottom->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.025f));

	NiagaraComponent->SetRelativeLocation(FVector(-32.0f, 0, 0));
	NiagaraComponent->SetRelativeRotation(FRotator(0, 180, 0));
	//// Location, Rotation, Scale 조정 ///// End


	//// StaticMesh 적용
	UStaticMesh* StaticMesh;
	CHelpers::GetAsset<UStaticMesh>(&StaticMesh, "StaticMesh'/Game/Meshes/Cylinder.Cylinder'");
	Cylinder->SetStaticMesh(StaticMesh);
	BodyRed->SetStaticMesh(StaticMesh);
	BodyRedLine1->SetStaticMesh(StaticMesh);
	BodyRedLine2->SetStaticMesh(StaticMesh);
	BodyRedTop->SetStaticMesh(StaticMesh);
	BodyRedBottom->SetStaticMesh(StaticMesh);

	CHelpers::GetAsset<UStaticMesh>(&StaticMesh, "StaticMesh'/Game/Meshes/Cone.Cone'");
	Cone->SetStaticMesh(StaticMesh);
	//// StaticMesh 적용 ///// End


	//// 메시 콜리전 삭제
	Cylinder->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
	Cone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyRed->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyRedLine1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyRedLine2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyRedTop->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyRedBottom->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//// 메시 콜리전 삭제 ///// End


	//// 나이아가라 애셋 적용
	UNiagaraSystem* ngSystem;
	CHelpers::GetAsset<UNiagaraSystem>(&ngSystem, "NiagaraSystem'/Game/Effects/NG_YonduTrail.NG_YonduTrail'");
	NiagaraComponent->SetAsset(ngSystem);
	//// 나이아가라 애셋 적용 //// End


	//// 사운드 애셋 적용
	USoundBase* soundBase;
	CHelpers::GetAsset<USoundBase>(&soundBase, "SoundCue'/Game/Sounds/Yondu_SoundCue.Yondu_SoundCue'");
	Sound->SetSound(soundBase);
	//// 사운드 애셋 적용 //// End


	//// 멤버 변수 조작
	InitialLifeSpan = 20.0f;
	Collision->SetSphereRadius(300.0f);		// Cone에 붙여 상속했으니 콜리전 범위가 작음 (0.6 * 0.2 * 300)
	Sound->bAutoActivate = false;
}

void ACSkill2Assassin::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACSkill2Assassin::OnComponentBeginOverlap);

	// 케릭터 스킬 소켓 위치로 이동
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	CheckNull(owner);

	FVector loc = owner->GetMesh()->GetSocketLocation("Assassin_SkillE_Right");
	FRotator rot = owner->GetActorRotation();

	FLatentActionInfo info;
	info.CallbackTarget = this;
	info.ExecutionFunction = "SpawnComplete";
	info.Linkage = 0;
	info.UUID = 0;
	UKismetSystemLibrary::MoveComponentTo(
		Scene,
		loc,
		rot,
		true,
		true,
		MoveTime,
		true,
		EMoveComponentAction::Type::Move,
		info
	);
}

void ACSkill2Assassin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 타겟이 없으면 Detection을 호충, 타겟이 0일 때는 owner 옆에 붙어있음
	if (Targets.Num() == 0)
		Detection();
}

void ACSkill2Assassin::Detection()
{
	// TODO: Sphere Trace로 Target 탐지 및 Detection 함수 호출

	hitCount = 0;					// 타겟 수만큼 증가해야 Detection함수 끝남

	////<< Start Trace
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	objectTypes.Add(Pawn);

	TArray<AActor*> ignores;
	ignores.Add(GetOwner());

	TArray<FHitResult> hitResults;

	UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation(),
		traceRadius,
		objectTypes,
		false,
		ignores,
		EDrawDebugTrace::None,
		hitResults,
		false
	);

	for (FHitResult hitResult : hitResults)
	{
		if (hitResult.bBlockingHit)
		{
			ACharacter* hitCharacter = Cast<ACharacter>(hitResult.GetActor());
			if (!!hitCharacter)
				Targets.AddUnique(hitCharacter);
		}
	}
	////<< End Trace

	
	// Attack 함수 호출
	AttackTarget();
}

void ACSkill2Assassin::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);

	// Hitted 배열에 추가 및 데미지 처리
	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (Hitted.Find(character) < 0)
	{
		Hitted.AddUnique(character);

		ACharacter* attacker = Cast<ACharacter>(GetOwner());
		UCWeaponComponent* weaponComp = CHelpers::GetComponent<UCWeaponComponent>(GetOwner());	// 공격자의 무기 히트데이터 가져오기 위함
		UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(OtherActor);		// 히트 대상
		if (!!weapon)
		{
			FVector dir = character->GetActorLocation() - GetActorLocation();
			FVector forVector = character->GetActorForwardVector();
			float dot = dir.DotProduct(dir, forVector);

			if (dot < 0)
				weapon->Damaged(attacker, this, &weaponComp->GetWeapon()->SkillEHitDatas[0]);	// 전방
			else
				weapon->Damaged(attacker, this, &weaponComp->GetWeapon()->SkillEHitDatas[1]);	// 후방
		}
	}
}

void ACSkill2Assassin::SpawnComplete()
{
	SetActorTickEnabled(true);
	NiagaraComponent->Deactivate();
}

void ACSkill2Assassin::AttackTarget()
{
	// TODO: hitCount가 타겟 수만큼 채워지지 않았다면 공격, 채워졌다면 owner옆으로 복귀 및 타겟 초기화
	Hitted.Empty();
	
	if (Targets.Num() > hitCount)
	{
		// TODO: 타겟을 향해 날아감, hitCount 증가

		NiagaraComponent->Activate();
		float randPitch = UKismetMathLibrary::RandomFloatInRange(1.0f, 3.0f);
		Sound->SetPitchMultiplier(randPitch);
		Sound->Play();

		FVector currLoc = GetActorLocation();
		FVector targetLoc = Targets[hitCount]->GetActorLocation();

		FRotator rot = UKismetMathLibrary::FindLookAtRotation(currLoc, targetLoc);			// 타겟을 바라보도록 회전
		float distance = GetDistanceTo(Targets[hitCount]) + 200.0f;							// 케릭터를 뚫고 지나가도록 200을 더해줌
		FVector direction = UKismetMathLibrary::GetDirectionUnitVector(currLoc, targetLoc);	// 시작지점에서 끝지점으로 방향 벡터 구함

		SetActorRotation(rot);	// 타겟을 바라보도록 메시 회전

		FLatentActionInfo info;
		info.CallbackTarget = this;
		info.ExecutionFunction = "AttackTarget";	// 콜백함수로 사용되려면 직렬화(UFUNCTION) 필수@@
		info.Linkage = 0;
		info.UUID = GetNextUUID();
		UKismetSystemLibrary::MoveComponentTo(	// 타겟을 뚫고 지나가도록 움직임
			Scene,
			currLoc + (direction * distance),
			rot,
			true,
			true,
			MoveTime,
			true,		// 최단 회전 경로
			EMoveComponentAction::Type::Move,
			info
		);

		hitCount++;
	}
	else if (Targets.Num() == hitCount)
	{
		// TODO: owner 옆으로 복귀

		NiagaraComponent->Deactivate();

		ACharacter* owner = Cast<ACharacter>(GetOwner());
		CheckNull(owner);

		FVector loc = owner->GetMesh()->GetSocketLocation("Assassin_SkillE_Right");
		FRotator rot;
		if (loc != GetActorLocation())
			rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), loc);
		else
			rot = GetOwner()->GetActorRotation();	// 스폰 위치와 같으면 케릭터와 같은 방향을 바라봄

		FLatentActionInfo info;
		info.CallbackTarget = this;
		info.ExecutionFunction = "ClearTarget";
		info.Linkage = 0;
		UKismetSystemLibrary::MoveComponentTo(
			Scene,
			loc,
			rot,
			true,
			true,
			MoveTime * 2,
			true,
			EMoveComponentAction::Type::Move,
			info
		);
	}
}

void ACSkill2Assassin::ClearTarget()
{
	// TODO: 타겟 초기화, hitCount 초기화, FLatentActionInfo UUID 초기화, 
	Targets.Empty();
	hitCount = 0;
	nextUUID = 0;
}

void ACSkill2Assassin::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	NiagaraComponent->Deactivate();
	Hitted.Empty();
	Targets.Empty();

	Super::EndPlay(EndPlayReason);
}