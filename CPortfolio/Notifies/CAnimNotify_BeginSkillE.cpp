#include "Notifies/CAnimNotify_BeginSkillE.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Gameframework/Character.h"

FString UCAnimNotify_BeginSkillE::GetNotifyName_Implementation() const
{
	return "BeginSkillE";
}

void UCAnimNotify_BeginSkillE::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	weapon->Begin_SkillE();
}

