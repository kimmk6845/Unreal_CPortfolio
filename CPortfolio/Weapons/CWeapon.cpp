#include "Weapons/CWeapon.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 생성자
ACWeapon::ACWeapon()
{
	CHelpers::GetAsset<UAnimMontage>(&DashMontage, L"AnimMontage'/Game/GhostLady_S1/Montages/Common/Dash_Unarmed_Montage.Dash_Unarmed_Montage'");

}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Begin Play
//// TODO: 데이터 테이블에서 데이터 로드 및 컴포넌트 로드
void ACWeapon::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());
	// 케릭터는 기본적으로 State, Status, Weapon Component를 가짐
	State = CHelpers::GetComponent<UCStateComponent>(Character);
	Status = CHelpers::GetComponent<UCStatusComponent>(Character);
	Weapon = CHelpers::GetComponent<UCWeaponComponent>(Character);

	{	// Do Action
		if (!!DoActionDataTable)
		{
			TArray<FName> rowNames = DoActionDataTable->GetRowNames();
			FString contextString(TEXT("DoActionData Context"));

			for (FName rowName : rowNames)
			{
				FDoActionDatas* rowData = DoActionDataTable->FindRow<FDoActionDatas>(rowName, contextString);
				DoActionDatas.Add(*rowData);
			}
		}

		if (!!HitDataTable)
		{
			TArray<FName> rowNames = HitDataTable->GetRowNames();
			FString contextString(TEXT("HitData Context"));

			for (FName rowName : rowNames)
			{
				FHitDatas* rowData = HitDataTable->FindRow<FHitDatas>(rowName, contextString);
				HitDatas.Add(*rowData);
			}
		}
	}// Do Action


	{	// Air Combo
		if (!!AirComboDataTable)
		{
			TArray<FName> rowNames = AirComboDataTable->GetRowNames();
			FString contextString(TEXT("AirComboData Context"));

			for (FName rowName : rowNames)
			{
				FAirComboDatas* rowData = AirComboDataTable->FindRow<FAirComboDatas>(rowName, contextString);
				AirComboDatas.Add(*rowData);
			}
		}

		if (!!AirHitDataTable)
		{
			TArray<FName> rowNames = AirHitDataTable->GetRowNames();
			FString contextString(TEXT("AirHitDatas Context"));

			for (FName rowName : rowNames)
			{
				FHitDatas* rowData = AirHitDataTable->FindRow<FHitDatas>(rowName, contextString);
				AirHitDatas.Add(*rowData);
			}
		}
	}// Air Combo

	{	// Skiill HitData
		if (!!SkillQHitDataTable)
		{
			TArray<FName> rowNames = SkillQHitDataTable->GetRowNames();
			FString contextString(TEXT("SkillQHitDatas Context"));

			for (FName rowName : rowNames)
			{
				FHitDatas* rowData = SkillQHitDataTable->FindRow<FHitDatas>(rowName, contextString);
				SkillQHitData = *rowData;
			}
		}

		if (!!SkillEHitDataTable)
		{
			TArray<FName> rowNames = SkillEHitDataTable->GetRowNames();
			FString contextString(TEXT("SkillEHitDatas Context"));

			for (FName rowName : rowNames)
			{
				FHitDatas* rowData = SkillEHitDataTable->FindRow<FHitDatas>(rowName, contextString);
				SkillEHitDatas.Add(*rowData);
			}
		}
	}// Skiill HitData
}
///////////////////////////////////////////////////////////////////////////////////////////////////



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
/* =========================================== PUBLIC =========================================== */

///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ 무기 장착 함수
//// TODO: ControlRotaionYaw 변수 적용, CanMove 변수 적용, 장착 애니메이션
void ACWeapon::Equip()
{
	State->SetEquipMode();

	if (bUseControlRotation == true)
		Status->EnableControlRotation();
	else
		Status->DisableControlRotation();

	if (bCanMove == true)
		Status->Move();
	else
		Status->Stop();

	if (!!EquipMontage)
	{
		Character->PlayAnimMontage(EquipMontage, Equip_MontagePlayRatio);
	}
	else
	{
		Begin_Equip();
		End_Equip();
	}
}

//// @ 장착 애니메이션 노티파이에서 호출될 함수
void ACWeapon::Begin_Equip()
{
	bInEquipping = true;
}
void ACWeapon::End_Equip()
{
	State->SetIdleMode();
	Status->Move();
	bInEquipping = false;
	bInEquip = true;
}

//// @ 무기 해제 함수
//// TODO: CanMove 변수 적용, 해제 애니메이션
void ACWeapon::Unequip()
{
	State->SetEquipMode();

	if (bCanMove == true)
		Status->Move();
	else
		Status->Stop();

	if (!!UnEquipMontage)
		Character->PlayAnimMontage(UnEquipMontage, Equip_MontagePlayRatio);
	else
		End_Equip();
}

//// @ 해제 애니메이션 노티파이에서 호출될 함수
void ACWeapon::End_UnEquip()
{
	State->SetIdleMode();
	Status->Move();
	Status->DisableControlRotation();
	bInEquip = false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ DoAction 노티파이 함수
//// TODO: InAction변수 조작, 케릭터 상태 조작
void ACWeapon::Begin_DoAction()
{
	bInAction = true;
}

void ACWeapon::End_DoAction()
{
	State->SetIdleMode();
	Status->Move();
	Status->DisableFixedCamera();
	bInAction = false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ Q Skill Function (Input Q Key)
//// TODO: 케릭터 모드 변경, 애니메이션 출력
void ACWeapon::SkillQ()
{
	CheckNull(SkillQMontage);

	if (State->IsIdleMode() == true)
	{
		State->SetSkillQMode();

		Character->PlayAnimMontage(SkillQMontage, SkillQ_PlayRatio);
	}
}

void ACWeapon::End_SkillQ()
{
	State->SetIdleMode();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//// @ E Skill Function (Input E Key)
//// TODO: 케릭터 모드 변경, 애니메이션 출력
void ACWeapon::SkillE()
{
	CheckNull(SkillEMontage);

	if (State->IsIdleMode() == true)
	{
		State->SetSkillEMode();

		Character->PlayAnimMontage(SkillEMontage, SkillE_PlayRatio);
	}
}

void ACWeapon::End_SkillE()
{
	State->SetIdleMode();
}
///////////////////////////////////////////////////////////////////////////////////////////////////


/* =========================================== PUBLIC =========================================== */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
