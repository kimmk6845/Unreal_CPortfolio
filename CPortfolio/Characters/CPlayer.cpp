#include "Characters/CPlayer.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "NiagaraComponent.h"

#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CFeetComponent.h"
#include "Components/CDashComponent.h"

#include "Engine/GameEngine.h"	// ###
ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// 컴포넌트
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);
	CHelpers::CreateComponent<UNiagaraComponent>(this, &NGDashComponent, "NGDashComponent", GetCapsuleComponent());
	
	CHelpers::CreateActorComponent<UCFeetComponent>(this, &FeetComponent, "FeetComp");
	CHelpers::CreateActorComponent<UCDashComponent>(this, &DashComponent, "DashComp");

	// 변수 세팅
	bJumping = ACharacter::bPressedJump;

	SpringArm->SetRelativeLocation(FVector(0, 0, 170));
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset = FVector(0, 60, 0);
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 7.0f;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	MaxHealth = 300.0f;

	// 메시
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/GhostLady_S1/Meshes/Characters/Combines/SK_AssassinC.SK_AssassinC'");
	GetMesh()->SetSkeletalMesh(mesh);

	// 애니메이션
	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/Character/Player/BP_CAnimInstance.BP_CAnimInstance_C'");
	GetMesh()->SetAnimClass(animInstance);

	// 대시 이펙트
	CHelpers::GetAsset<UNiagaraSystem>(&DashEffect, L"NiagaraSystem'/Game/Effects/NG_Dash.NG_Dash'");
	UNiagaraSystem* ngSystem;
	CHelpers::GetAsset<UNiagaraSystem>(&ngSystem, L"NiagaraSystem'/Game/Effects/NG_Dash.NG_Dash'");
	NGDashComponent->SetAsset(ngSystem);
	NGDashComponent->bAutoActivate = false;
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerCameraManager* cameraMgr = GetController<APlayerController>()->PlayerCameraManager;
	cameraMgr->ViewPitchMin = PitchRange.X;
	cameraMgr->ViewPitchMax = PitchRange.Y;

	Health = MaxHealth;
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 속도 보간
	if (bSprint == true && GetVelocity().Size2D() > 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = UKismetMathLibrary::FInterpTo(CurrSpeed, 600.0f, DeltaTime, 2);
		CurrSpeed = GetCharacterMovement()->MaxWalkSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = UKismetMathLibrary::FInterpTo(CurrSpeed, 300.0f, DeltaTime, 2);
		CurrSpeed = GetCharacterMovement()->MaxWalkSpeed;
	}

	if (bUsedDash == true)
	{
		DashCoolTime += DeltaTime;
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, FString::SanitizeFloat(DashCoolTime));
		if (DashCoolTime >= InitDashCoolTime)
		{
			bUsedDash = false;
			DashCoolTime = 0.0f;
		}
	}
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 축 매핑
	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);

	// 액션 매핑
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACPlayer::CheckJump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACPlayer::Dash);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACPlayer::CheckJump);

	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ACPlayer::Begin_Run);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ACPlayer::End_Run);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::DoAction);
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Released, this, &ACPlayer::End_Fire);
	PlayerInputComponent->BindAction("OneHand", EInputEvent::IE_Pressed, this, &ACPlayer::OneHand);
	PlayerInputComponent->BindAction("Assassin", EInputEvent::IE_Pressed, this, &ACPlayer::Assassin);
	PlayerInputComponent->BindAction("Rifle", EInputEvent::IE_Pressed, this, &ACPlayer::Rifle);

	PlayerInputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &ACPlayer::Reload);
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Pressed, this, &ACPlayer::Begin_SubAction);
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Released, this, &ACPlayer::End_SubAction);

	PlayerInputComponent->BindAction("Skill_Q", EInputEvent::IE_Pressed, this, &ACPlayer::SkillQ);
	PlayerInputComponent->BindAction("Skill_E", EInputEvent::IE_Pressed, this, &ACPlayer::SkillE);
	PlayerInputComponent->BindAction("SubSkill", EInputEvent::IE_Pressed, this, &ACPlayer::SubSkill);

	PlayerInputComponent->BindAction("Avoid", EInputEvent::IE_Pressed, this, &ACPlayer::Avoid);		// Ctrl + Space bar
	// PlayerInputComponent->BindAction("Avoid", EInputEvent::IE_Released, this, &ACPlayer::Avoid);
}

///// Movement & Camera Rotation /////
void ACPlayer::OnMoveForward(float AxisValue)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector().GetSafeNormal2D();

	if (StatusComponent->IsCanMove() == true)
		AddMovementInput(direction, AxisValue);
}

void ACPlayer::OnMoveRight(float AxisValue)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector().GetSafeNormal2D();

	if (StatusComponent->IsCanMove() == true)
		AddMovementInput(direction, AxisValue);
}

void ACPlayer::OnVerticalLook(float AxisValue)
{
	if(StatusComponent->IsFixedCamera() == false)
		AddControllerPitchInput(AxisValue);
}

void ACPlayer::OnHorizontalLook(float AxisValue)
{
	if (StatusComponent->IsFixedCamera() == false)
		AddControllerYawInput(AxisValue);
}
////////////////////////////////////////



///// Sprint /////
void ACPlayer::Begin_Run()
{
	bSprint = true;
}

void ACPlayer::End_Run()
{
	bSprint = false;
}
//////////////////


///// Jump /////
void ACPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	jumpCount = 0;
	StateComponent->SetIdleMode();
}

void ACPlayer::CheckJump()
{
	float vel = GetVelocity().Size2D();
	if (vel >= InitVel)		// 대쉬 속도면 점프를 안함
		return;

	if (StateComponent->IsIdleMode() == true || StateComponent->IsJumpMode() == true)
	{
		if (bJumping == false)
		{
			StateComponent->SetJumpMode();

			bJumping = true;
			jumpCount++;
			if (jumpCount == 2)
				LaunchCharacter(FVector(0, 0, 500), false, true);
		}
		else
			bJumping = false;

		ACharacter::bPressedJump = bJumping;
	}
}
//////////////////

///// Action /////
void ACPlayer::DoAction()
{
	Super::DoAction();
}

void ACPlayer::End_Fire()
{
	Super::End_Fire();

}
//////////////////


///// Equip /////
void ACPlayer::OneHand()
{
	Super::OneHand();
}

void ACPlayer::Assassin()
{
	Super::Assassin();
}

void ACPlayer::Rifle()
{
	Super::Rifle();
}
//////////////////


void ACPlayer::Reload()
{
	Super::Reload();
}

void ACPlayer::Begin_SubAction()
{
	Super::Begin_SubAction();
}

void ACPlayer::End_SubAction()
{
	Super::End_SubAction();
}

///// SKILL //////
void ACPlayer::SkillQ()
{
	Super::SkillQ();
}

void ACPlayer::SkillE()
{
	Super::SkillE();
}

void ACPlayer::SubSkill()
{
	Super::SubSkill();
}
//////////////////


///// AVOID //////
void ACPlayer::Avoid()
{
	Super::Avoid();
}

//////////////////


///// DASH //////
void ACPlayer::Dash()
{
	float vel = GetVelocity().Size2D();
	if (vel >= InitVel && bUsedDash == false)
	{
		// ### 위젯(프로그래스 바)	--> 쿨타임일 때 visible, 사용 가능시 Invisible
		// ### 대쉬 어택
		bUsedDash = true;

		NGDashComponent->SetActive(true);
		// 대쉬 컴포넌트를 통해 대쉬 몽타주랑 거리 조절
		float dashDistance = DashComponent->GetDistance();
		LaunchCharacter(GetVelocity() * dashDistance, false, false);
		PlayAnimMontage(DashComponent->GetMontage());

		// ### State컴포넌트랑 연동해서 상태 변경시키는 것도 있어야 할 듯
	}
}

// 노티파이 함수
void ACPlayer::End_Dash()
{
	NGDashComponent->Deactivate();
	// ### 상태 변경
}
//////////////////
