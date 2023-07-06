#include "Notifies/CAnimNotify_EndSkillQ.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Gameframework/Character.h"

FString UCAnimNotify_EndSkillQ::GetNotifyName_Implementation() const
{
	return "EndSkillQ";
}

void UCAnimNotify_EndSkillQ::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	weapon->End_SkillQ();
}

