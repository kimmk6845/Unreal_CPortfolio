#include "Notifies/CAnimNotify_BeginDoAction.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Gameframework/Character.h"

FString UCAnimNotify_BeginDoAction::GetNotifyName_Implementation() const
{
	return "BeginDoAction";
}

void UCAnimNotify_BeginDoAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	weapon->Begin_DoAction();
}
