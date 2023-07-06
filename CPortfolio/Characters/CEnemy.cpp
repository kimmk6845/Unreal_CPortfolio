#include "Characters/CEnemy.h"
#include "Global.h"
#include "Weapons/CWeapon.h"
#include "CAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 300.0f;

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Enemy/Meshes/SK_AssassinMerged.SK_AssassinMerged'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/Character/Enemy/BP_CEnemy_AnimInstance.BP_CEnemy_AnimInstance_C'");
	GetMesh()->SetAnimClass(animInstance);
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
	