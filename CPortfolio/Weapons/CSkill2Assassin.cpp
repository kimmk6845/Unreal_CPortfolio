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
	PrimaryActorTick.bStartWithTickEnabled = false;		// �ʱ� ƽ�� Off. ������ġ�� Ȱ��ȭ �Ǹ� tick Ȱ��ȭ

	//// ������Ʈ ����
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
	//// ������Ʈ ���� ///// End


	//// Location, Rotation, Scale ����
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
	//// Location, Rotation, Scale ���� ///// End


	//// StaticMesh ����
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
	//// StaticMesh ���� ///// End


	//// �޽� �ݸ��� ����
	Cylinder->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
	Cone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyRed->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyRedLine1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyRedLine2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyRedTop->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyRedBottom->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//// �޽� �ݸ��� ���� ///// End


	//// ���̾ư��� �ּ� ����
	UNiagaraSystem* ngSystem;
	CHelpers::GetAsset<UNiagaraSystem>(&ngSystem, "NiagaraSystem'/Game/Effects/NG_YonduTrail.NG_YonduTrail'");
	NiagaraComponent->SetAsset(ngSystem);
	//// ���̾ư��� �ּ� ���� //// End


	//// ���� �ּ� ����
	USoundBase* soundBase;
	CHelpers::GetAsset<USoundBase>(&soundBase, "SoundCue'/Game/Sounds/Yondu_SoundCue.Yondu_SoundCue'");
	Sound->SetSound(soundBase);
	//// ���� �ּ� ���� //// End


	//// ��� ���� ����
	InitialLifeSpan = 20.0f;
	Collision->SetSphereRadius(300.0f);		// Cone�� �ٿ� ��������� �ݸ��� ������ ���� (0.6 * 0.2 * 300)
	Sound->bAutoActivate = false;
}

void ACSkill2Assassin::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACSkill2Assassin::OnComponentBeginOverlap);

	// �ɸ��� ��ų ���� ��ġ�� �̵�
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

	// Ÿ���� ������ Detection�� ȣ��, Ÿ���� 0�� ���� owner ���� �پ�����
	if (Targets.Num() == 0)
		Detection();
}

void ACSkill2Assassin::Detection()
{
	// TODO: Sphere Trace�� Target Ž�� �� Detection �Լ� ȣ��

	hitCount = 0;					// Ÿ�� ����ŭ �����ؾ� Detection�Լ� ����

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

	
	// Attack �Լ� ȣ��
	AttackTarget();
}

void ACSkill2Assassin::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);

	// Hitted �迭�� �߰� �� ������ ó��
	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (Hitted.Find(character) < 0)
	{
		Hitted.AddUnique(character);

		ACharacter* attacker = Cast<ACharacter>(GetOwner());
		UCWeaponComponent* weaponComp = CHelpers::GetComponent<UCWeaponComponent>(GetOwner());	// �������� ���� ��Ʈ������ �������� ����
		UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(OtherActor);		// ��Ʈ ���
		if (!!weapon)
		{
			FVector dir = character->GetActorLocation() - GetActorLocation();
			FVector forVector = character->GetActorForwardVector();
			float dot = dir.DotProduct(dir, forVector);

			if (dot < 0)
				weapon->Damaged(attacker, this, &weaponComp->GetWeapon()->SkillEHitDatas[0]);	// ����
			else
				weapon->Damaged(attacker, this, &weaponComp->GetWeapon()->SkillEHitDatas[1]);	// �Ĺ�
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
	// TODO: hitCount�� Ÿ�� ����ŭ ä������ �ʾҴٸ� ����, ä�����ٸ� owner������ ���� �� Ÿ�� �ʱ�ȭ
	Hitted.Empty();
	
	if (Targets.Num() > hitCount)
	{
		// TODO: Ÿ���� ���� ���ư�, hitCount ����

		NiagaraComponent->Activate();
		float randPitch = UKismetMathLibrary::RandomFloatInRange(1.0f, 3.0f);
		Sound->SetPitchMultiplier(randPitch);
		Sound->Play();

		FVector currLoc = GetActorLocation();
		FVector targetLoc = Targets[hitCount]->GetActorLocation();

		FRotator rot = UKismetMathLibrary::FindLookAtRotation(currLoc, targetLoc);			// Ÿ���� �ٶ󺸵��� ȸ��
		float distance = GetDistanceTo(Targets[hitCount]) + 200.0f;							// �ɸ��͸� �հ� ���������� 200�� ������
		FVector direction = UKismetMathLibrary::GetDirectionUnitVector(currLoc, targetLoc);	// ������������ ���������� ���� ���� ����

		SetActorRotation(rot);	// Ÿ���� �ٶ󺸵��� �޽� ȸ��

		FLatentActionInfo info;
		info.CallbackTarget = this;
		info.ExecutionFunction = "AttackTarget";	// �ݹ��Լ��� ���Ƿ��� ����ȭ(UFUNCTION) �ʼ�@@
		info.Linkage = 0;
		info.UUID = GetNextUUID();
		UKismetSystemLibrary::MoveComponentTo(	// Ÿ���� �հ� ���������� ������
			Scene,
			currLoc + (direction * distance),
			rot,
			true,
			true,
			MoveTime,
			true,		// �ִ� ȸ�� ���
			EMoveComponentAction::Type::Move,
			info
		);

		hitCount++;
	}
	else if (Targets.Num() == hitCount)
	{
		// TODO: owner ������ ����

		NiagaraComponent->Deactivate();

		ACharacter* owner = Cast<ACharacter>(GetOwner());
		CheckNull(owner);

		FVector loc = owner->GetMesh()->GetSocketLocation("Assassin_SkillE_Right");
		FRotator rot;
		if (loc != GetActorLocation())
			rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), loc);
		else
			rot = GetOwner()->GetActorRotation();	// ���� ��ġ�� ������ �ɸ��Ϳ� ���� ������ �ٶ�

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
	// TODO: Ÿ�� �ʱ�ȭ, hitCount �ʱ�ȭ, FLatentActionInfo UUID �ʱ�ȭ, 
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