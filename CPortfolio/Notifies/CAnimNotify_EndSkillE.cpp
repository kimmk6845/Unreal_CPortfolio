#include "Notifies/CAnimNotify_EndSkillE.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Gameframework/Character.h"

FString UCAnimNotify_EndSkillE::GetNotifyName_Implementation() const
{
	return "EndSkillE";
}

void UCAnimNotify_EndSkillE::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	weapon->End_SkillE();
}

