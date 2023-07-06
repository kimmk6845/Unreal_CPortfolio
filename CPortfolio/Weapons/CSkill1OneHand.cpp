#include "Weapons/CSkill1OneHand.h"
#include "Global.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "CWeapon.h"
#include "Components/CWeaponComponent.h"

ACSkill1OneHand::ACSkill1OneHand()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Collision, "Collision", Scene);
	CHelpers::CreateComponent<UNiagaraComponent>(this, &NiagaraComponent, "NiagaraComponent", Scene);

	UNiagaraSystem* ngSystem;
	CHelpers::GetAsset<UNiagaraSystem>(&ngSystem, "NiagaraSystem'/Game/Effects/BG_OneHandSkillq.BG_OneHandSkillQ'");
	NiagaraComponent->SetAsset(ngSystem);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->InitialSpeed = 4000.0f;
	ProjectileMovement->bAutoActivate = false;
	
	Collision->SetRelativeScale3D(FVector(1, 1, 7));
	NiagaraComponent->SetRelativeLocation(FVector(0,150,-300));
	NiagaraComponent->SetRelativeRotation(FRotator(0, -90, 0));
	NiagaraComponent->SetRelativeScale3D(FVector(1, 1, 2));

	InitialLifeSpan = 3.0f;
}

void ACSkill1OneHand::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACSkill1OneHand::OnComponentBeginOverlap);
	
}

void ACSkill1OneHand::Shoot(FVector InDirection, FRotator InRotation)
{
	Scene->SetRelativeRotation(InRotation);

	FVector velocity = ProjectileMovement->InitialSpeed * InDirection;
	ProjectileMovement->Velocity = velocity;

	ProjectileMovement->SetActive(true);
}

// 데미지 처리
void ACSkill1OneHand::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (Hitted.Find(character) < 0)
	{
		Hitted.AddUnique(character);

		ACharacter* attacker = Cast<ACharacter>(GetOwner());
		UCWeaponComponent* weaponComp = CHelpers::GetComponent<UCWeaponComponent>(GetOwner());	// 공격자의 무기 히트데이터 가져오기 위함
		UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(OtherActor);		// 히트 대상
		if (!!weapon)
		{
			weapon->Damaged(attacker, this, &weaponComp->GetWeapon()->SkillQHitData);
		}
	}
}

void ACSkill1OneHand::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	NiagaraComponent->Deactivate();
	Hitted.Empty();
}