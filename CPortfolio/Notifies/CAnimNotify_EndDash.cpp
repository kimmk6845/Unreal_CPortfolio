#include "Notifies/CAnimNotify_EndDash.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Gameframework/Character.h"

FString UCAnimNotify_EndDash::GetNotifyName_Implementation() const
{
	return "EndDash";
}

void UCAnimNotify_EndDash::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->End_Dash();
}

