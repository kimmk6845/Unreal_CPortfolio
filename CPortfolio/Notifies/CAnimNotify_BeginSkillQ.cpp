#include "Notifies/CAnimNotify_BeginSkillQ.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Gameframework/Character.h"

FString UCAnimNotify_BeginSkillQ::GetNotifyName_Implementation() const
{
	return "BeginSkillQ";
}

void UCAnimNotify_BeginSkillQ::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	weapon->Begin_SkillQ();
}
