#include "Weapons/CWeapon_Melee_OneHand.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStatusComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gameframework/Character.h"
#include "CSkill1OneHand.h"
#include "CSkill2OneHand.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 생성자
ACWeapon_Melee_OneHand::ACWeapon_Melee_OneHand()
{
	CHelpers::GetAsset<UAnimMontage>(&EquipMontage, L"AnimMontage'/Game/GhostLady_S1/Montages/OneHand/OneHand_Equip_Montage.OneHand_Equip_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&UnEquipMontage, L"AnimMontage'/Game/GhostLady_S1/Montages/OneHand/OneHand_Unequip_Montage.OneHand_Unequip_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&SkillQMontage, L"AnimMontage'/Game/GhostLady_S1/Montages/OneHand/OneHand_Skill1_Montage.OneHand_Skill1_Montage'");
	CHelpers::GetAsset<UAnimMontage>(&SkillEMontage, L"AnimMontage'/Game/GhostLady_S1/Montages/OneHand/OneHand_Skill2_Montage.OneHand_Skill2_Montage'");
	
	CHelpers::GetAsset<UDataTable>(&DoActionDataTable, L"DataTable'/Game/BP/Weapons/OneHand/OneHand_DoAction.OneHand_DoAction'");
	CHelpers::GetAsset<UDataTable>(&HitDataTable, L"DataTable'/Game/BP/Weapons/OneHand/OneHand_HitData.OneHand_HitData'");
	CHelpers::GetAsset<UDataTable>(&AirComboDataTable, L"DataTable'/Game/BP/Weapons/OneHand/OneHand_AirCombo.OneHand_AirCombo'");
	CHelpers::GetAsset<UDataTable>(&AirHitDataTable, L"DataTable'/Game/BP/Weapons/OneHand/OneHand_AirHitData.OneHand_AirHitData'");
	CHelpers::GetAsset<UDataTable>(&SkillQHitDataTable, L"DataTable'/Game/BP/Weapons/OneHand/OneHand_SkillQHitData.OneHand_SkillQHitData'");
	CHelpers::GetAsset<UDataTable>(&SkillEHitDataTable, L"DataTable'/Game/BP/Weapons/OneHand/OneHand_SkillEHitData.OneHand_SkillEHitData'");
	bUseControlRotation = false;

	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Sheath, "Sheath", RootComponent);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Sword, "Sword", Sheath);
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Collision, "Collision", Sword);

	UStaticMesh* mesh;
	CHelpers::GetAsset<UStaticMesh>(&mesh, L"StaticMesh'/Game/WeaponMesh/OneHand/OneHand_Sheath.OneHand_Sheath'");
	Sheath->SetStaticMesh(mesh);
	Sheath->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CHelpers::GetAsset<UStaticMesh>(&mesh, L"StaticMesh'/Game/WeaponMesh/OneHand/OneHand_Sword.OneHand_Sword'");
	Sword->SetStaticMesh(mesh);
	Sword->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Collision->SetRelativeLocation(FVector(0, -40.0f, 0));
	Collision->SetRelativeRotation(FRotator(0, 0, -90.0f));
	Collision->SetCapsuleHalfHeight(90.0f);
	CollisionRadius = 15.0f;
	Collision->SetCapsuleRadius(CollisionRadius);

	HolsterSocketName = "OneHand_Holster";
	EquipSocketName = "OneHand_RightHand";

	CHelpers::GetClass(&SkillQClass, "Blueprint'/Game/BP/Weapons/OneHand/BP_CSkill1OneHand.BP_CSkill1OneHand_C'");
	CHelpers::GetClass(&GhostTrailClass, "Blueprint'/Game/BP/Effect/BP_CGhostTrail.BP_CGhostTrail_C'");

	CHelpers::GetClass(&SkillEClass, "Blueprint'/Game/BP/Weapons/OneHand/BP_CSkill2OneHand.BP_CSkill2OneHand_C'");

}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ BeginPlay
//// TODO: 칼 메시를 케릭터 소켓에 붙임
void ACWeapon_Melee_OneHand::BeginPlay()
{
	Super::BeginPlay();

	Sheath->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, HolsterSocketName);
	Sword->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, HolsterSocketName);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Tick
//// TODO: 공중 콤보일 때 스프링암 조절
void ACWeapon_Melee_OneHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Weapon->IsOneHandMode() && !!springArm)
	{
		if (bInAir == true)
		{
			springArm->TargetArmLength = UKismetMathLibrary::FInterpTo(springArm->TargetArmLength, InitSpringArmLength * 1.5, DeltaTime, 1.5f);
		}
		else
		{
			springArm->TargetArmLength = UKismetMathLibrary::FInterpTo(springArm->TargetArmLength, InitSpringArmLength, DeltaTime, 1.5f);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* =========================================== PRIVATE =========================================== */


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 장착 노티파이 함수
//// TODO: 칼을 케릭터 손 위치로 붙임
void ACWeapon_Melee_OneHand::Begin_Equip()
{
	Super::Begin_Equip();

	Sword->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, EquipSocketName);

}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 해제 노티파이 함수
//// TODO: 칼을 칼집 위치에 맞는 케릭터 소켓에 붙임
void ACWeapon_Melee_OneHand::End_UnEquip()
{
	Super::End_UnEquip();
	
	Sword->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, HolsterSocketName);

}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 스킬 Q 노티파이 함수
//// TODO: Q 스킬 애니메이션에 맞게 투사체 스킬을 전방으로 발사
void ACWeapon_Melee_OneHand::Begin_SkillQ()
{
	Super::Begin_SkillQ();

	CheckNull(SkillQClass);

	FVector direction;
	direction = Character->GetActorForwardVector();

	FVector location;
	location = Character->GetMesh()->GetSocketLocation("OneHand_RightHand");

	FRotator rotation;
	rotation = Character->GetMesh()->GetSocketRotation("OneHand_RightHand");
	
	FActorSpawnParameters params;
	params.Owner = Character;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform transform;
	transform.SetLocation(location);
	transform.SetRotation(FQuat(Character->GetActorRotation()));

	ACSkill1OneHand* skill;
	skill = Character->GetWorld()->SpawnActor<ACSkill1OneHand>(SkillQClass, transform, params);
	
	CheckNull(skill);
	skill->Shoot(direction, rotation);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Skill E (Input E Key)
//// TODO: 애니메이션 진행 중에는 Stop
void ACWeapon_Melee_OneHand::SkillE()
{
	Super::SkillE();

	Status->Stop();
	bSkilling = true;
	tempDatas = HitDatas;
	HitDatas = SkillEHitDatas;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ E 스킬 노티파이 함수
void ACWeapon_Melee_OneHand::Begin_SkillE()
{
	Super::Begin_SkillE();

	// TODO: 주변의 적 끌어당기는 것 (Tick으로 일정 주기마다? 혹은 약하게 계속 --> 액터를 스폰해야 함)
	// 검기 >> 콜리전 길이를 늘리거나 그냥 Sphere Collision으로 때려버리기?
	FTransform transform = Character->GetActorTransform();

	FActorSpawnParameters params;
	params.Owner = Character;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	skillE = Character->GetWorld()->SpawnActor<ACSkill2OneHand>(SkillEClass, transform, params);
	skillE->AttachToActor(Character, FAttachmentTransformRules::KeepWorldTransform, "pelvis");
	Collision->SetCapsuleHalfHeight(500.0f);	// ### 그냥 히트데이터가 들어감, 애니메이션도 부자연스러움
}

void ACWeapon_Melee_OneHand::End_SkillE()
{
	Super::End_SkillE();

	skillE->Destroy();
	Collision->SetCapsuleHalfHeight(90.0f);
	Status->Move();
	bSkilling = false;
	HitDatas = tempDatas;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


/* =========================================== PRIVATE =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
