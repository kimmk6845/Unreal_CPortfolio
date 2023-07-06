#include "Components/CStatusComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Characters/CCharacter.h"

UCStatusComponent::UCStatusComponent()
{

}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());
}

void UCStatusComponent::Move()
{
	bCanMove = true;
}

void UCStatusComponent::Stop()
{
	bCanMove = false;
}

void UCStatusComponent::EnableControlRotation()
{
	Character->bUseControllerRotationYaw = true;
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UCStatusComponent::DisableControlRotation()
{
	Character->bUseControllerRotationYaw = false;
	Character->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void UCStatusComponent::BackupControlRotation()
{
	bBackup_RotationYaw = Character->bUseControllerRotationYaw;
	bBackup_ToMovement = Character->GetCharacterMovement()->bOrientRotationToMovement;
}

void UCStatusComponent::RestoreControlRotation()
{
	Character->bUseControllerRotationYaw = bBackup_RotationYaw;
	Character->GetCharacterMovement()->bOrientRotationToMovement = bBackup_ToMovement;
}

float UCStatusComponent::GetHealth()
{
	ACCharacter* character = Cast<ACCharacter>(Character);

	return character->GetHealth();
}