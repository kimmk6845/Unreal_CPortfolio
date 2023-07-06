#include "Characters/CAnimInstance.h"
#include "Global.h"
#include "Characters/CCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACCharacter>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);

	Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	if (!!Weapon)
		Weapon->OnWeaponTypeChange.AddDynamic(this, &UCAnimInstance::OnWeaponTypeChanged);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CheckNull(OwnerCharacter);

	Speed = OwnerCharacter->GetVelocity().Size2D();
	Direction = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetControlRotation());
	Falling = OwnerCharacter->GetCharacterMovement()->IsFalling();
	
	JumpCount = OwnerCharacter->JumpCount();

	UCFeetComponent* feet = CHelpers::GetComponent<UCFeetComponent>(OwnerCharacter);
	if (!!feet)
	{
		bFeet = true;
		FeetData = feet->GetData();
	}
	else
		bFeet = false;

	SetPlayerState();
	SetWeaponState();


}

void UCAnimInstance::SetPlayerState()
{
	AimPitch = OwnerCharacter->GetAimPitch();
}

void UCAnimInstance::SetWeaponState()
{
	bInAim = Weapon->IsInAim();
	bFiring = Weapon->IsFiring();
	LeftHandLocation = Weapon->GetLeftHandLocation();

	bUseIK = WeaponType == EWeaponType::Rifle;
}


void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}


