#include "Notifies/CAnimNotifyState_RangeCollision.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Gameframework/Character.h"
#include "Weapons/CWeapon_Melee.h"

FString UCAnimNotifyState_RangeCollision::GetNotifyName_Implementation() const
{
	return "RangeCollision";
}

void UCAnimNotifyState_RangeCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	ACWeapon_Melee* weaponMelee = Cast<ACWeapon_Melee>(weapon->GetWeapon());
	weaponMelee->OnRangeCollision();
}

void UCAnimNotifyState_RangeCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	ACWeapon_Melee* weaponMelee = Cast<ACWeapon_Melee>(weapon->GetWeapon());
	weaponMelee->OffRangeCollision();
}

