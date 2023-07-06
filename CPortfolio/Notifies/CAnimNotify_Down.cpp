#include "Notifies/CAnimNotify_Down.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Gameframework/Character.h"
#include "Weapons/CWeapon_Melee.h"


FString UCAnimNotify_Down::GetNotifyName_Implementation() const
{
	return "Down";
}

void UCAnimNotify_Down::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	ACWeapon_Melee* weaponMelee = Cast<ACWeapon_Melee>(weapon->GetWeapon());
	weaponMelee->DownCharacter();

}
