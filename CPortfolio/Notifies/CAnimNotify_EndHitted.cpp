#include "Notifies/CAnimNotify_EndHitted.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Gameframework/Character.h"

FString UCAnimNotify_EndHitted::GetNotifyName_Implementation() const
{
	return "EndHitted";
}

void UCAnimNotify_EndHitted::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(state);

	state->SetIdleMode();
}
