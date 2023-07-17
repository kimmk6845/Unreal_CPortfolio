#include "Components/CDashComponent.h"
#include "Global.h"
#include "CWeaponComponent.h"
#include "Weapons/CWeapon.h"
#include "GameFramework/Character.h"

UCDashComponent::UCDashComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CHelpers::GetAsset<UAnimMontage>(&DashMontage, L"AnimMontage'/Game/GhostLady_S1/Montages/Common/Dash_Unarmed_Montage.Dash_Unarmed_Montage'");
	DashDistance = 10.0f;
}


void UCDashComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());

}


void UCDashComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

UAnimMontage* UCDashComponent::GetMontage()
{
	UCWeaponComponent* weaponComp = CHelpers::GetComponent<UCWeaponComponent>(Owner);
	if (!!weaponComp)
	{
		ACWeapon* weapon = weaponComp->GetWeapon();
		if (weaponComp->GetWeaponType() == EWeaponType::Unarmed)
			return DashMontage;

		if (!!weapon)
			return weapon->GetDashMontage();

	}

	return nullptr;
}

float UCDashComponent::GetDistance()
{
	UCWeaponComponent* weaponComp = CHelpers::GetComponent<UCWeaponComponent>(Owner);
	if (!!weaponComp)
	{
		ACWeapon* weapon = weaponComp->GetWeapon();
		if (weaponComp->GetWeaponType() == EWeaponType::Unarmed)
			return DashDistance;

		if (!!weapon)
			return weapon->GetDashDistance();

	}

	return 0.0f;
}