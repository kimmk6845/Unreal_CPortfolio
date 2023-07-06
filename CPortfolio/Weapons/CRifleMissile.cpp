#include "Weapons/CRifleMissile.h"
#include "Global.h"
#include "CExplosion.h"
#include "CSkill1Rifle.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CWeaponComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Gameframework/Character.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

ACRifleMissile::ACRifleMissile()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh", Scene);
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Collision, "Collision", Scene);
	CHelpers::CreateComponent<USphereComponent>(this, &ExplosionCollision, "ExplosionCollision", Scene);
	CHelpers::CreateComponent<UNiagaraComponent>(this, &NiagaraComponent, "Niagara", Scene);

	UStaticMesh* missile;
	CHelpers::GetAsset<UStaticMesh>(&missile, "StaticMesh'/Game/Effects/Meshes/SM_Rocket.SM_Rocket'");
	Mesh->SetStaticMesh(missile);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UNiagaraSystem* ngSystem;
	CHelpers::GetAsset<UNiagaraSystem>(&ngSystem, "NiagaraSystem'/Game/Effects/NS_ROCKET_Trail.NS_ROCKET_Trail'");
	NiagaraComponent->SetAsset(ngSystem);
	NiagaraComponent->SetRelativeLocation(FVector(-175.0f, 0.0f, 0.0f));
	NiagaraComponent->SetRelativeScale3D(FVector(3.0f));

	CHelpers::GetClass<ACExplosion>(&ExplosionClass, "Blueprint'/Game/BP/Effect/BP_CExplosion.BP_CExplosion_C'");

	CHelpers::GetClass<UMatineeCameraShake>(&ExplosionCameraShakeClass, "Blueprint'/Game/BP/Weapons/Rifle/BP_RifleSkill1_Explosion_CameraShake.BP_RifleSkill1_Explosion_CameraShake_C'");

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	ProjectileMovement->InitialSpeed = 500.0f;
	ProjectileMovement->MaxSpeed = 1000.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 5000.0f;
	ProjectileMovement->bAutoActivate = true;
	ProjectileMovement->Velocity = FVector(200.0f, 0.0f, 300.0f);

	Mesh->SetRelativeScale3D(FVector(20.0f, 5.0f, 5.0f));
	Collision->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	Collision->SetCapsuleRadius(44.0f);
	Collision->SetCapsuleHalfHeight(192.0f);

	ExplosionCollision->SetSphereRadius(5000.0f);

}

void ACRifleMissile::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACRifleMissile::OnComponentBeginOverlap);

	ExplosionCollision->OnComponentBeginOverlap.AddDynamic(this, &ACRifleMissile::OnComponentBeginOverlap2);
	ExplosionCollision->OnComponentEndOverlap.AddDynamic(this, &ACRifleMissile::OnComponentEndOverlap2);

	SetActorScale3D(FVector(0.1f));
}

void ACRifleMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!!Target)
	{
		FRotator rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
		SetActorRotation(rotation);
	}
}

void ACRifleMissile::Shoot(ACharacter* TargetActor)
{
	Target = TargetActor;

	USceneComponent* scene = CHelpers::GetComponent<USceneComponent>(Cast<AActor>(TargetActor));
	if (!!scene)
	{
		ProjectileMovement->HomingTargetComponent = scene;
	}
}

void ACRifleMissile::LoadData(FHitDatas* InHitData)
{
	HitData = *InHitData;
}

void ACRifleMissile::GetOwner(ACSkill1Rifle* InOwner)
{
	Owner = InOwner;
}

void ACRifleMissile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) == OtherActor);

	// 미사일 발사하자마자 폭발하는 것 방지
	ACRifleMissile* otherMissile = Cast<ACRifleMissile>(OtherActor);
	if (!!otherMissile)
		return;
	ACSkill1Rifle* skill = Cast<ACSkill1Rifle>(OtherActor);
	if (!!skill)
		return;

	if (!!ExplosionCameraShakeClass)
	{
		APlayerController* controller = GetWorld()->GetFirstPlayerController();
		if (!!controller)
			controller->PlayerCameraManager->StartCameraShake(ExplosionCameraShakeClass);
	}

	if (!!ExplosionClass)
	{
		ACExplosion* missile = GetWorld()->SpawnActor<ACExplosion>(ExplosionClass, GetActorLocation(), GetActorRotation());
	}

	// 데미지 처리를 missile에서 하면 Destroy되면서 적의 애니메이션이 제대로 실행되지 않음
	// 그래서 다시 Rifle Skill로 돌아가서 데미지 처리
	Owner->ExplosionDamage(OverlapCharacters);

	Destroy();
}

void ACRifleMissile::OnComponentBeginOverlap2(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) == OtherActor);

	ACharacter* Enemy = Cast<ACharacter>(OtherActor);
	CheckNull(Enemy);

	if (OverlapCharacters.Find(Enemy) < 0)
		OverlapCharacters.AddUnique(Enemy);
}

void ACRifleMissile::OnComponentEndOverlap2(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* Enemy = Cast<ACharacter>(OtherActor);
	CheckNull(Enemy);
	if (OverlapCharacters.Find(Enemy) < 0)
		return;

	OverlapCharacters.Remove(Enemy);
}