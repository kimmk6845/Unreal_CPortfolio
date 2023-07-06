#include "Components/CWeaponComponent.h"
#include "Global.h"
#include "Weapons/CWeapon.h"
#include "Weapons/CWeapon_Rifle.h"
#include "Characters/CCharacter.h"
#include "Gameframework/Character.h"
#include "Characters/CPlayer.h"
#include "Widgets/CUserWidget_HUD.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 생성자
UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CHelpers::GetClass<UCUserWidget_HUD>(&HUDClass, "WidgetBlueprint'/Game/BP/Widgets/WB_HUD.WB_HUD_C'");
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Begin Play
//// TODO: WeaponClass의 무기들을 Weapons 변수에 로드하고 무기들 스폰 및 HUD 생성
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
//// TODO: HUD 업데이트
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
//// @ 무기 장착 함수
//// TODO: Weapon의 무기 장착 함수 호출
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
//// @ 무기 해제 함수
//// TODO: Weapon의 무기 해제 함수 호출
void UCWeaponComponent::End_UnEquip()
{
	CheckNull(Weapon);
	Weapon->End_UnEquip();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Hand IK
//// ToDO: 케릭터 왼손의 위치 가져옴
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
//// @ DoAction (입력: 좌클릭)
//// TODO: Weapon의 Action함수 호출
void UCWeaponComponent::DoAction()
{
	CheckNull(Weapon);
	if (IsUnarmedMode() == false)
		Weapon->DoAction();
}

//// @ 애니메이션 몽타주에서 호출할 함수
//// TODO: 콤보 입력을 위한 함수로 Weapon의 Begin, End Action을 호출
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

//// @ 무기가 Rifle일 때 Click Released 함수
//// TODO: 클릭 Release시 사격을 멈춤
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
//// TODO: 조준중인지 체크
bool UCWeaponComponent::IsInAim()
{
	if (IsRifleMode() == true)
	{
		ACWeapon_Rifle* rifle = Cast<ACWeapon_Rifle>(Weapon);

		return rifle->IsInAim();
	}

	return false;
}

//// TODO: 사격중인지 체크
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
//// @ Reload 함수 (입력: R)
//// TODO: rifle의 reload함수 호출. 사격중지, 모드 변경, 애니메이션 출력
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

//// @ 노티파이 함수
//// TODO: 탄창 제거 (bone에서 숨김 및 떨어지는 탄창 스폰)
void UCWeaponComponent::Eject_Magazine()
{
	CheckNull(Weapon);
	ACWeapon_Rifle* rifle = Cast<ACWeapon_Rifle>(Weapon);

	if (!!rifle)
	{
		rifle->Eject_Magazine();
	}
}

//// @ 노티파이 함수
//// TODO: 탄창 생성 (재장전될 탄창 허리춤에서 본에 붙임)
void UCWeaponComponent::Spawn_Magazine()
{
	CheckNull(Weapon);
	ACWeapon_Rifle* rifle = Cast<ACWeapon_Rifle>(Weapon);

	if (!!rifle)
	{
		rifle->Spawn_Magazine();
	}
}

//// @ 노티파이 함수
//// TODO: 탄창 생성 (손에 붙은 탄창 제거 및 총에서 bone 숨김 해제)
void UCWeaponComponent::Load_Magazine()
{
	CheckNull(Weapon);
	ACWeapon_Rifle* rifle = Cast<ACWeapon_Rifle>(Weapon);

	if (!!rifle)
	{
		rifle->Load_Magazine();
	}
}

//// @ 노티파이 함수
//// TODO: 모드 적용 및 탄창 수 증가
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
//// TODO: Weapon의 SubAction함수 호출. 공중 콤보 사전 동작
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
//// TODO: Weapon의 SkillQ함수 호출. 
void UCWeaponComponent::SkillQ()
{
	CheckNull(Weapon);
	Weapon->SkillQ();
}

//// @ 노티파이 함수
void UCWeaponComponent::Begin_SkillQ()
{
	CheckNull(Weapon);
	Weapon->Begin_SkillQ();
}

//// @ 노티파이 함수
void UCWeaponComponent::End_SkillQ()
{
	CheckNull(Weapon);
	Weapon->End_SkillQ();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ E Skill Function (Input E Key)
//// TODO: Weapon의 SkillE함수 호출. 
void UCWeaponComponent::SkillE()
{
	CheckNull(Weapon);
	Weapon->SkillE();
}

//// @ 노티파이 함수
void UCWeaponComponent::Begin_SkillE()
{
	CheckNull(Weapon);
	Weapon->Begin_SkillE();
}

//// @ 노티파이 함수
void UCWeaponComponent::End_SkillE()
{
	CheckNull(Weapon);
	Weapon->End_SkillE();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ SubSkill Function (Input X Key)
//// TODO: Weapon의 SubSkill함수 호출.
void UCWeaponComponent::SubSkill()
{
	CheckNull(Weapon);
	Weapon->SubSkill();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 데미지 전달 함수
//// @ param 1: 공격자 케릭터
//// @ param 2: 공격 매개체 (무기)
//// @ param 3: 피격 정보 구조체
//// TODO: 케릭터 클래스의 Damaged함수 호출
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
//// @ 무기 변경 내부 함수
//// @ param 1: 변경 무기 타입
//// TODO: 델리게이트 등록 함수 호출
void UCWeaponComponent::ChangeType(EWeaponType inType)
{
	EWeaponType PrevType = Type;
	Type = inType;

	if (OnWeaponTypeChange.IsBound())
		OnWeaponTypeChange.Broadcast(PrevType, inType);
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 무기 변경 내부 함수
//// @ param 1: 변경 무기 타입
//// TODO: 현재 무기를 장착 해제하고 인자로 들어온 무기 타입으로 장착
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
