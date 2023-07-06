#include "Notifies/CAnimNotify_Launch.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Gameframework/Character.h"
#include "Weapons/CWeapon_Melee.h"


FString UCAnimNotify_Launch::GetNotifyName_Implementation() const
{
	return "Launch";
}

void UCAnimNotify_Launch::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	ACWeapon_Melee* weaponMelee = Cast<ACWeapon_Melee>(weapon->GetWeapon());
	weaponMelee->LaunchCharacter();

}
