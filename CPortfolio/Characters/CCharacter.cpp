#include "Characters/CCharacter.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"


ACCharacter::ACCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &WeaponComponent, "WeaponComp");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &StateComponent, "StateComp");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &StatusComponent, "StatusComp");

	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));

	CurrSpeed = 300.0f;
	GetCharacterMovement()->MaxWalkSpeed = CurrSpeed;
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	jumpCount = 0;
}

void ACCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	StateComponent->OnStateTypeChanged.AddDynamic(this, &ACCharacter::OnStateTypeChanged);

}

void ACCharacter::Damaged(ACharacter* InAttacker, AActor* InCauser, FHitDatas* InHitData)
{
	Damage.Attacker = InAttacker;
	Damage.Causer = InCauser;
	Damage.HitData = InHitData;

	StateComponent->SetHittedMode();
}


void ACCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator prev = FRotator(AimPitch, AimYaw, 0);
	FRotator curr = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation());

	FRotator delta = UKismetMathLibrary::RInterpTo(prev, curr, DeltaTime, ViewInterpSpeed);
	AimPitch = FMath::Clamp(delta.Pitch, PitchRange.X, PitchRange.Y);
	AimYaw = FMath::Clamp(delta.Yaw, YawRange.X, YawRange.Y);
}

void ACCharacter::PlayAnimation(ACharacter* InOwner)
{
	CheckNull(Damage.HitData->HitMontage);

	if(GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() == false)
		InOwner->PlayAnimMontage(Damage.HitData->HitMontage, Damage.HitData->Hit_MontagePlayRatio);
}
void ACCharacter::ForcePlayAnimation(ACharacter* InOwner)
{
	CheckNull(Damage.HitData->HitMontage);

	InOwner->PlayAnimMontage(Damage.HitData->HitMontage, Damage.HitData->Hit_MontagePlayRatio);
}
void ACCharacter::PlayHitStop(UWorld* InWorld)
{
	CheckTrue(FMath::IsNearlyZero(Damage.HitData->HitStop));

	TArray<ACharacter*> characters;
	for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
	{
		ACharacter* character = Cast<ACharacter>(actor);

		if (!!character)
		{
			character->CustomTimeDilation = 1e-2f;

			characters.Add(character);
		}
	}


	FTimerDelegate timerDelegate = FTimerDelegate::CreateLambda
	(
		[=]()
		{
			for (ACharacter* character : characters)
				character->CustomTimeDilation = 1.0f;
		}
	);


	FTimerHandle handle;
	InWorld->GetTimerManager().SetTimer(handle, timerDelegate, Damage.HitData->HitStop, false);

}
void ACCharacter::PlaySound(ACharacter* InOwner)
{
	CheckNull(Damage.HitData->Sound);

	UWorld* world = InOwner->GetWorld();
	FVector location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Damage.HitData->Sound, location);
}
void ACCharacter::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	FTransform transform;
	transform.SetLocation(InLocation);
	transform.SetScale3D(Damage.HitData->EffectScale);
	float randYaw = FMath::FRandRange(0, 360);
	transform.SetRotation(FQuat(FRotator(0, randYaw, 0)));
	transform.AddToTranslation(Damage.HitData->EffectLocation);

	if (!!Damage.HitData->Effect)	// 파티클
	{
		CHelpers::PlayEffect(InWorld, Damage.HitData->Effect, transform);
		return;
	}

	if (!!Damage.HitData->NGEffect)		// 나이아가라
	{
		CHelpers::PlayEffect(InWorld, Damage.HitData->NGEffect, transform);
		return;
	}

}
void ACCharacter::PlayCameraShake()
{
	if (!!Damage.HitData->ShakeClass)
	{
		APlayerController* controller = GetWorld()->GetFirstPlayerController();
		if (!!controller)
		{
			controller->PlayerCameraManager->StartCameraShake(Damage.HitData->ShakeClass);
		}
	}
}


///// Action /////
void ACCharacter::DoAction()
{
	if (StateComponent->IsJumpMode() == false)
		WeaponComponent->DoAction();
}

void ACCharacter::End_Fire()
{
	WeaponComponent->End_Fire();
}
//////////////////

///// Equip /////
void ACCharacter::OneHand()
{
	if (StateComponent->IsIdleMode() == true)
		WeaponComponent->SetOneHandMode();
}

void ACCharacter::Assassin()
{
	if (StateComponent->IsIdleMode() == true)
		WeaponComponent->SetAssassinMode();
}

void ACCharacter::Rifle()
{
	if (StateComponent->IsIdleMode() == true)
		WeaponComponent->SetRifleMode();
}
//////////////////

void ACCharacter::Reload()
{
	if (StateComponent->IsIdleMode() == true)
	{
		WeaponComponent->Reload();
		StateComponent->SetReloadMode();
	}
}

void ACCharacter::Begin_SubAction()
{
	if (StateComponent->IsJumpMode() == false)
		WeaponComponent->Begin_SubAction();
}

void ACCharacter::End_SubAction()
{
	if (StateComponent->IsJumpMode() == false)
		WeaponComponent->End_SubAction();
}

///// SKILL //////
void ACCharacter::SkillQ()
{
	if (StateComponent->IsIdleMode() == true)
		WeaponComponent->SkillQ();
}

void ACCharacter::SubSkill()
{
	if (StateComponent->IsSkillQMode() == true)
		WeaponComponent->SubSkill();
}

void ACCharacter::SkillE()
{
	if (StateComponent->IsIdleMode() == true)
		WeaponComponent->SkillE();
}
//////////////////


///// AVOID //////
void ACCharacter::Avoid()
{
	CLog::Print("AVOID");
}

//////////////////


// State Type Change
void ACCharacter::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Hitted: Hitted(); break;
	case EStateType::Dead: Dead(); break;
	}

}

void ACCharacter::Hitted()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	ACCharacter* rifleCharacter = Cast<ACCharacter>(Damage.Attacker);

	FVector start = GetActorLocation();
	FVector target = Damage.Attacker->GetActorLocation();
	FVector direction = target - start;
	direction.Normalize();

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(start, target);
	FRotator origin = GetActorRotation();

	Health = Health - Damage.HitData->Damage;

	CLog::Print(Health);

	if (Health <= 0)
	{
		StateComponent->SetDeadMode();
		LaunchCharacter(-direction * Damage.HitData->Launch, false, false);
		return;
	}

	if (!!rifleCharacter)	// 라이플 타격
	{
		if (rifleCharacter->WeaponComponent->IsRifleMode() == true)
		{
			if (!!Damage.HitData)
			{
				FHitDatas* hitData = Damage.HitData;

				PlayAnimation(this);
				SetActorRotation(FRotator(origin.Pitch, rotator.Yaw, origin.Roll));
			}
			return;
		}
	}

	if (!!Damage.HitData)
	{
		FHitDatas* hitData = Damage.HitData;

		PlayAnimation(this);
		PlayHitStop(GetWorld());
		PlaySound(this);
		PlayEffect(GetWorld(), GetActorLocation());
		PlayCameraShake();

		LaunchCharacter(-direction * Damage.HitData->Launch, false, false);

		SetActorRotation(FRotator(origin.Pitch, rotator.Yaw, origin.Roll));
	}
}

void ACCharacter::Dead()
{
	// GetMesh()->SetCollisionProfileName("Ragdoll");
	// GetMesh()->SetSimulatePhysics(true);
	SetLifeSpan(5);
}
