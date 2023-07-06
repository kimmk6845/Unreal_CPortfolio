#include "Components/CWeaponComponent.h"
#include "Global.h"
#include "Weapons/CWeapon.h"
#include "Weapons/CWeapon_Rifle.h"
#include "Characters/CCharacter.h"
#include "Gameframework/Character.h"
#include "Characters/CPlayer.h"
#include "Widgets/CUserWidget_HUD.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ������
UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CHelpers::GetClass<UCUserWidget_HUD>(&HUDClass, "WidgetBlueprint'/Game/BP/Widgets/WB_HUD.WB_HUD_C'");
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Begin Play
//// TODO: WeaponClass�� ������� Weapons ������ �ε��ϰ� ����� ���� �� HUD ����
void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());

	FActorSpawnParameters params;
	params.Owner = Owner;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	for (TSubclassOf<ACWeapon> weaponClass : WeaponClasses)
	{
		if (!!weaponClass)
		{
			ACWeapon* weapon = Owner->GetWorld()->SpawnActor<ACWeapon>(weaponClass, params);
			Weapons.Add(weapon);
		}
	}

	if (!!HUDClass)
	{
		ACPlayer* player = Cast<ACPlayer>(Owner);
		if (!!player)
		{
			HUD = CreateWidget<UCUserWidget_HUD, APlayerController>(Owner->GetController<APlayerController>(), HUDClass);
			HUD->AddToViewport();
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Tick
//// TODO: HUD ������Ʈ
void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ACPlayer* player = Cast<ACPlayer>(Owner);
	if (!!player)
	{
		if (!!HUD)
		{
			if (IsRifleMode() == true)
			{
				CheckNull(Weapon);
				ACWeapon_Rifle* rifle = Cast<ACWeapon_Rifle>(Weapon);

				uint8 currMagazineCount = rifle->GetCurrMagazineCount();
				uint8 maxMagazineCount = rifle->GetMaxMagazineCount();

				HUD->VisibleAmmo();
				HUD->UpdateMagazine(currMagazineCount, maxMagazineCount);
			}
			else
				HUD->HiddenAmmo();
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* =========================================== PUBLIC =========================================== */


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Setter
void UCWeaponComponent::SetUnarmedMode()
{
	if (IsUnarmedMode() == false)
	{
		Weapon->Unequip();
		ChangeType(EWeaponType::Unarmed);
	}
}
void UCWeaponComponent::SetOneHandMode()
{
	SetMode(EWeaponType::OneHand);
}
void UCWeaponComponent::SetAssassinMode()
{
	SetMode(EWeaponType::Assassin);
}
void UCWeaponComponent::SetRifleMode()
{
	SetMode(EWeaponType::Rifle);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ���� ���� �Լ�
//// TODO: Weapon�� ���� ���� �Լ� ȣ��
void UCWeaponComponent::Begin_Equip()
{
	CheckNull(Weapon);
	Weapon->Begin_Equip();
}
void UCWeaponComponent::End_Equip()
{
	CheckNull(Weapon);
	Weapon->End_Equip();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ���� ���� �Լ�
//// TODO: Weapon�� ���� ���� �Լ� ȣ��
void UCWeaponComponent::End_UnEquip()
{
	CheckNull(Weapon);
	Weapon->End_UnEquip();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Hand IK
//// ToDO: �ɸ��� �޼��� ��ġ ������
FVector UCWeaponComponent::GetLeftHandLocation()
{
	if (!!Weapon)
	{
		ACWeapon_Rifle* rifle = Cast<ACWeapon_Rifle>(Weapon);

		if (!!rifle)
			return rifle->GetLeftHandLocation();

	}

	return FVector(0, 0, 0);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ DoAction (�Է�: ��Ŭ��)
//// TODO: Weapon�� Action�Լ� ȣ��
void UCWeaponComponent::DoAction()
{
	CheckNull(Weapon);
	if (IsUnarmedMode() == false)
		Weapon->DoAction();
}

//// @ �ִϸ��̼� ��Ÿ�ֿ��� ȣ���� �Լ�
//// TODO: �޺� �Է��� ���� �Լ��� Weapon�� Begin, End Action�� ȣ��
void UCWeaponComponent::Begin_DoAction()
{
	CheckNull(Weapon);
	Weapon->Begin_DoAction();
}
void UCWeaponComponent::End_DoAction()
{
	CheckNull(Weapon);
	Weapon->End_DoAction();
}

void UCWeaponComponent::RandomDoAction()
{
	CheckNull(Weapon);
	Weapon->RandomDoAction();
}

//// @ ���Ⱑ Rifle�� �� Click Released �Լ�
//// TODO: Ŭ�� Release�� ����� ����
void UCWeaponComponent::End_Fire()
{
	CheckNull(Weapon);
	if (IsRifleMode() == true)
	{
		ACWeapon_Rifle* rifle = Cast<ACWeapon_Rifle>(Weapon);

		rifle->End_Fire();
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Check Function
//// TODO: ���������� üũ
bool UCWeaponComponent::IsInAim()
{
	if (IsRifleMode() == true)
	{
		ACWeapon_Rifle* rifle = Cast<ACWeapon_Rifle>(Weapon);

		return rifle->IsInAim();
	}

	return false;
}

//// TODO: ��������� üũ
bool UCWeaponComponent::IsFiring()
{
	if (IsRifleMode() == true)
	{
		ACWeapon_Rifle* rifle = Cast<ACWeapon_Rifle>(Weapon);

		return rifle->IsFiring();
	}

	return false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Reload �Լ� (�Է�: R)
//// TODO: rifle�� reload�Լ� ȣ��. �������, ��� ����, �ִϸ��̼� ���
void UCWeaponComponent::Reload()
{
	CheckNull(Weapon);
	ACWeapon_Rifle* rifle = Cast<ACWeapon_Rifle>(Weapon);

	if (!!rifle)
	{
		CheckFalse(rifle->CanReload());

		rifle->Reload();
	}
}

//// @ ��Ƽ���� �Լ�
//// TODO: źâ ���� (bone���� ���� �� �������� źâ ����)
void UCWeaponComponent::Eject_Magazine()
{
	CheckNull(Weapon);
	ACWeapon_Rifle* rifle = Cast<ACWeapon_Rifle>(Weapon);

	if (!!rifle)
	{
		rifle->Eject_Magazine();
	}
}

//// @ ��Ƽ���� �Լ�
//// TODO: źâ ���� (�������� źâ �㸮�㿡�� ���� ����)
void UCWeaponComponent::Spawn_Magazine()
{
	CheckNull(Weapon);
	ACWeapon_Rifle* rifle = Cast<ACWeapon_Rifle>(Weapon);

	if (!!rifle)
	{
		rifle->Spawn_Magazine();
	}
}

//// @ ��Ƽ���� �Լ�
//// TODO: źâ ���� (�տ� ���� źâ ���� �� �ѿ��� bone ���� ����)
void UCWeaponComponent::Load_Magazine()
{
	CheckNull(Weapon);
	ACWeapon_Rifle* rifle = Cast<ACWeapon_Rifle>(Weapon);

	if (!!rifle)
	{
		rifle->Load_Magazine();
	}
}

//// @ ��Ƽ���� �Լ�
//// TODO: ��� ���� �� źâ �� ����
void UCWeaponComponent::End_Reload()
{
	CheckNull(Weapon);
	ACWeapon_Rifle* rifle = Cast<ACWeapon_Rifle>(Weapon);

	if (!!rifle)
	{
		rifle->End_Reload();
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ SubAction (Right Click Function)
//// TODO: Weapon�� SubAction�Լ� ȣ��. ���� �޺� ���� ����
void UCWeaponComponent::Begin_SubAction()
{
	CheckNull(Weapon);
	Weapon->Begin_SubAction();
}
void UCWeaponComponent::End_SubAction()
{
	CheckNull(Weapon);
	Weapon->End_SubAction();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Q Skill Function (Input Q Key)
//// TODO: Weapon�� SkillQ�Լ� ȣ��. 
void UCWeaponComponent::SkillQ()
{
	CheckNull(Weapon);
	Weapon->SkillQ();
}

//// @ ��Ƽ���� �Լ�
void UCWeaponComponent::Begin_SkillQ()
{
	CheckNull(Weapon);
	Weapon->Begin_SkillQ();
}

//// @ ��Ƽ���� �Լ�
void UCWeaponComponent::End_SkillQ()
{
	CheckNull(Weapon);
	Weapon->End_SkillQ();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ E Skill Function (Input E Key)
//// TODO: Weapon�� SkillE�Լ� ȣ��. 
void UCWeaponComponent::SkillE()
{
	CheckNull(Weapon);
	Weapon->SkillE();
}

//// @ ��Ƽ���� �Լ�
void UCWeaponComponent::Begin_SkillE()
{
	CheckNull(Weapon);
	Weapon->Begin_SkillE();
}

//// @ ��Ƽ���� �Լ�
void UCWeaponComponent::End_SkillE()
{
	CheckNull(Weapon);
	Weapon->End_SkillE();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ SubSkill Function (Input X Key)
//// TODO: Weapon�� SubSkill�Լ� ȣ��.
void UCWeaponComponent::SubSkill()
{
	CheckNull(Weapon);
	Weapon->SubSkill();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ������ ���� �Լ�
//// @ param 1: ������ �ɸ���
//// @ param 2: ���� �Ű�ü (����)
//// @ param 3: �ǰ� ���� ����ü
//// TODO: �ɸ��� Ŭ������ Damaged�Լ� ȣ��
void UCWeaponComponent::Damaged(ACharacter* InAttacker, AActor* InCauser, FHitDatas* InHitData)
{
	ACCharacter* character = Cast<ACCharacter>(Owner);
	CheckNull(character);

	character->Damaged(InAttacker, InCauser, InHitData);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


/* =========================================== PUBLIC =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//




//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* =========================================== PRIVATE =========================================== */


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ���� ���� ���� �Լ�
//// @ param 1: ���� ���� Ÿ��
//// TODO: ��������Ʈ ��� �Լ� ȣ��
void UCWeaponComponent::ChangeType(EWeaponType inType)
{
	EWeaponType PrevType = Type;
	Type = inType;

	if (OnWeaponTypeChange.IsBound())
		OnWeaponTypeChange.Broadcast(PrevType, inType);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ ���� ���� ���� �Լ�
//// @ param 1: ���� ���� Ÿ��
//// TODO: ���� ���⸦ ���� �����ϰ� ���ڷ� ���� ���� Ÿ������ ����
void UCWeaponComponent::SetMode(EWeaponType inType)
{
	if (Type == inType)
		SetUnarmedMode();
	else
	{
		if (IsUnarmedMode() == false)
		{
			Weapon->Unequip();
			SetUnarmedMode();
			return;
		}
		
		Weapon = Weapons[(int32)inType];
		CheckNull(Weapon);

		Weapon->Equip();
		ChangeType(inType);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////



/* =========================================== PRIVATE =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
