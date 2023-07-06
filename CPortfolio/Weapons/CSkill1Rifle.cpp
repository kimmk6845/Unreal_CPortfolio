#include "Weapons/CSkill1Rifle.h"
#include "Global.h"
#include "CBullet.h"
#include "CRifleMissile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "CWeapon.h"
#include "Gameframework/Character.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"



ACSkill1Rifle::ACSkill1Rifle()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene", RootComponent);
	CHelpers::CreateComponent<USphereComponent>(this, &Collision, "Collision", Scene);
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh", Scene);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &MissileLeft, "MissileLeft", Scene);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &MissileRight, "MissileRight", Scene);

	Collision->SetSphereRadius(MaxRadius);

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/VigilanteContent/Vehicles/West_Fighter_F18C/SK_West_Fighter_F18C.SK_West_Fighter_F18C'");
	Mesh->SetSkeletalMesh(mesh);

	Mesh->SetRelativeRotation(FRotator(0, 180, 0));
	Mesh->SetRelativeScale3D(FVector(0.08f));

	CHelpers::GetAsset<UParticleSystem>(&FlashParticle, "ParticleSystem'/Game/Effects/P_Muzzleflash.P_Muzzleflash'");
	CHelpers::GetAsset<USoundWave>(&FireSound, "SoundWave'/Game/Sounds/S_RifleShoot.S_RifleShoot'");
	CHelpers::GetAsset<USoundWave>(&MissileSound, "SoundWave'/Game/Sounds/MissileLaunch.MissileLaunch'");

	CHelpers::GetClass<ACBullet>(&BulletClass, "Blueprint'/Game/BP/Weapons/Rifle/BP_CBullet.BP_CBullet_C'");
	CHelpers::GetClass<ACRifleMissile>(&MissileClass, "Blueprint'/Game/BP/Weapons/Rifle/BP_CRifleMissile.BP_CRifleMissile_C'");

	CHelpers::GetAsset<UMaterialInstanceConstant>(&HitDecal, "MaterialInstanceConstant'/Game/Materials/M_Decal_Inst.M_Decal_Inst'");
	CHelpers::GetAsset<UParticleSystem>(&HitParticle, "ParticleSystem'/Game/Effects/P_Impact_Default.P_Impact_Default'");
	CHelpers::GetAsset<UNiagaraSystem>(&HitNiagara, "NiagaraSystem'/Game/Effects/NG_Hit.NG_Hit'");
	CHelpers::GetClass<UMatineeCameraShake>(&CameraShakeClass, "Blueprint'/Game/BP/Weapons/Rifle/BP_RifleSkill1_CameraShake.BP_RifleSkill1_CameraShake_C'");
	CHelpers::GetAsset<UDataTable>(&SkillQHitDataTable, L"DataTable'/Game/BP/Weapons/Rifle/Rifle_QSkillHitData.Rifle_QSkillHitData'");

	UStaticMesh* missile;
	CHelpers::GetAsset<UStaticMesh>(&missile, "StaticMesh'/Game/Effects/Meshes/SM_Rocket.SM_Rocket'");
	MissileLeft->SetStaticMesh(missile);
	MissileRight->SetStaticMesh(missile);

	InitialLifeSpan = 30.0f;

	FlashBoneName = "MachineGunFire";
}

void ACSkill1Rifle::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < Mesh->GetMaterials().Num(); i++)
	{
		UMaterialInstanceDynamic* instance = UMaterialInstanceDynamic::Create(Mesh->GetMaterials()[i], this);
		Mesh->SetMaterial(i, instance);
	}


	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACSkill1Rifle::OnComponentBeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ACSkill1Rifle::OnComponentEndOverlap);

	CurrentAmmo = MaxAmmo;

	MissileLeft->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform, "Missile_Left");
	MissileRight->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform, "Missile_Right");

	{	// Skiill HitData
		if (!!SkillQHitDataTable)
		{
			TArray<FName> rowNames = SkillQHitDataTable->GetRowNames();
			FString contextString(TEXT("SkillQHitDatas Context"));

			for (FName rowName : rowNames)
			{
				FHitDatas* rowData = SkillQHitDataTable->FindRow<FHitDatas>(rowName, contextString);
				SkillQHitDatas.Add(*rowData);
			}
		}
	}// Skiill HitData
}

void ACSkill1Rifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Appear Particle
	if (MaterialScalarValue <= 0.6f)
	{
		for (UMaterialInterface* material : Mesh->GetMaterials())
		{
			UMaterialInstanceDynamic* instance = Cast<UMaterialInstanceDynamic>(material);

			if (!!instance)
			{
				MaterialScalarValue = MaterialScalarValue + DeltaTime * 0.5f;
				instance->SetScalarParameterValue("Dissolve", MaterialScalarValue);
			}
		}
	}

	// 플레이어와 가장 가까이에 있는 케릭터를 감지
	float actorLen = MaxRadius;	
	for (ACharacter* overlapActor : OverlapActors)
	{
		FVector vectorLen = GetOwner()->GetActorLocation() - overlapActor->GetActorLocation();

		if (actorLen > vectorLen.Size())
		{
			actorLen = vectorLen.Size();
			Candidate = overlapActor;
		}
	}

	// 가장 가까이에 있는 케릭터 바라봄
	if (OverlapActors.Num() > 0)
	{
		if (!!Candidate)
		{
			FRotator rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Candidate->GetActorLocation());
			SetActorRotation(rotation);
		}
	}
	else 
	{
		Candidate = nullptr;
		FRotator rotation = Mesh->GetRelativeRotation();
		rotation.Yaw = rotation.Yaw + DeltaTime * 90.0f;
		Mesh->SetRelativeRotation(rotation);
		actorLen = MaxRadius;
	}

	// 반복문 도중 그냥 삭제하면 에러발생함
	for (int32 i = OverlapActors.Num() - 1;i >= 0; i--)
	{
		if (!!OverlapActors[i])
		{
			UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(OverlapActors[i]);

			// bAllowShrinking 변수를 false로 만들어
			// 공간을 해제하고 재할당하는 것을 막아 에러 발생을 막음
			if (status->GetHealth() <= 0)
			{
				const bool bAllowShrinking = false;
				OverlapActors.RemoveAt(i, 1, bAllowShrinking);
			}
		}
	}
}

void ACSkill1Rifle::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);

	ACSkill1Rifle* skill = Cast<ACSkill1Rifle>(OtherActor);
	if (!!skill)
		return;

	ACharacter* character = Cast<ACharacter>(OtherActor);
	CheckNull(character);
	if (OverlapActors.Find(character) < 0)
	{
		OverlapActors.AddUnique(character);
	}

	if (OverlapActors.Num() == 1)
	{
		Begin_Fire();
		Mesh->SetRelativeRotation(FRotator(0, 180, 0));  // 메시 회전 버그 픽스..
	}

}

void ACSkill1Rifle::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);
	CheckNull(character);
	if (OverlapActors.Find(character) < 0)
		return;

	OverlapActors.Remove(character);
	if (OverlapActors.Num() == 0)
		End_Fire();
}

void ACSkill1Rifle::Begin_Fire()
{
	// TODO: OnFiring 타이머로 호출
	if (OverlapActors.Num() > 0 && bReloading == false)
	{
		GetWorld()->GetTimerManager().SetTimer(AutoFireHandle, this, &ACSkill1Rifle::OnFiring, AutoFireInterval, true, 0);
	}
}

void ACSkill1Rifle::End_Fire()
{
	// TODO: 타이머 제거
	GetWorld()->GetTimerManager().ClearTimer(AutoFireHandle);
}

void ACSkill1Rifle::OnFiring()
{
	// 주변 적이 사망하면 End_Fire
	if (OverlapActors.Num() == 0)
		End_Fire();
	
	// TODO: 공격 코드
	CurrentAmmo--;

	FVector direction = GetActorForwardVector();
	FTransform transform = GetActorTransform();

	FVector start = transform.GetLocation() + direction;

	direction = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(direction, 3);	// 탄 퍼뜨림
	FVector end = transform.GetLocation() + direction * HitDistance;


	TArray<AActor*> ignores;
	ignores.Add(GetOwner());
	ignores.Add(this);

	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, params);
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
				ACharacter* attacker = Cast<ACharacter>(GetOwner());
				weapon->Damaged(attacker, this, &SkillQHitDatas[0]);
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
		UGameplayStatics::SpawnEmitterAttached(FlashParticle, Mesh, FlashBoneName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);

	FVector FireLocation = Mesh->GetSocketLocation(FlashBoneName);

	if (!!FireSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, FireLocation);

	if (!!BulletClass)
	{
		FVector spawnLocation = FireLocation + direction;

		ACBullet* bullet = GetWorld()->SpawnActor<ACBullet>(BulletClass, spawnLocation, direction.Rotation());
		bullet->Shoot(direction);
	}

	if (!!CameraShakeClass)
	{
		APlayerController* controller = Cast<ACharacter>(GetOwner())->GetController<APlayerController>();
		if (!!controller)
			controller->PlayerCameraManager->StartCameraShake(CameraShakeClass);
	}

	if (CurrentAmmo <= 0)
	{
		Reload();
		End_Fire();
	}
}

void ACSkill1Rifle::Reload()
{
	bReloading = true;
	CurrentAmmo = MaxAmmo;

	Explosion();
}

void ACSkill1Rifle::Explosion()
{
	FTimerHandle WaitHandle;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()	
		{
			bReloading = false;
			Begin_Fire();
		}), ReloadTime, false);

	int index = 0;
	for (ACharacter* actor : OverlapActors)
	{
		index = index % 2;
		FVector direction = UKismetMathLibrary::Normal(actor->GetActorLocation() - GetActorLocation());
		FTransform transform = GetActorTransform();

		if (!!FlashParticle)
			UGameplayStatics::SpawnEmitterAttached(FlashParticle, Mesh, MissileBoneName[index], FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);

		FVector FireLocation = Mesh->GetSocketLocation(MissileBoneName[index]);

		if (!!MissileSound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), MissileSound, FireLocation);

		if (!!MissileClass)
		{
			FVector spawnLocation = FireLocation + direction;

			ACRifleMissile* missile = GetWorld()->SpawnActor<ACRifleMissile>(MissileClass, spawnLocation, GetActorRotation());
			if (!!missile)
			{
				missile->Shoot(actor);
				missile->LoadData(&SkillQHitDatas[1]);
				missile->GetOwner(this);
			}
		}
		index++;
	}
}

void ACSkill1Rifle::ExplosionDamage(TArray<ACharacter*> InHitCharacters)
{
	for (ACharacter* hitCharacter : InHitCharacters)
	{
		UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(hitCharacter);
		CheckNull(weapon);

		ACharacter* attacker = Cast<ACharacter>(GetOwner());
		weapon->Damaged(attacker, this, &SkillQHitDatas[1]);
	}
}