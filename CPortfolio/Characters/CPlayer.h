#pragma once

#include "CoreMinimal.h"
#include "Characters/CCharacter.h"
#include "GenericTeamAgentInterface.h"   // �� ID ���
#include "CPlayer.generated.h"

UCLASS()
class CPORTFOLIO_API ACPlayer 
	: public ACCharacter
	, public IGenericTeamAgentInterface // �������̽� ��� 

{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		class UCFeetComponent* FeetComponent;

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// ��Ʈ�ѷ� ������� �ʱ� ������ ���⼭ �Է� ó��
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//�ܺο� Team ID�� �������ִ� �Լ� 
	FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(TeamID); }

private:
	// Movement & CameraRotation
	void OnMoveForward(float AxisValue);
	void OnMoveRight(float AxisValue);
	void OnVerticalLook(float AxisValue);
	void OnHorizontalLook(float AxisValue);

private:
	// Sprint
	void Begin_Run();
	void End_Run();

private:
	// Jump
	virtual void Landed(const FHitResult& Hit) override;
	void CheckJump();

private:
	// Action
	void DoAction() override;
	void End_Fire() override;
	void OneHand() override;
	void Assassin() override;
	void Rifle() override;

private:
	void Reload() override;
	void Begin_SubAction() override;		// Rifle�� Aim
	void End_SubAction() override;

private:
	void SkillQ() override;
	void SkillE() override;
	void SubSkill() override;

private:
	void Avoid() override;


// ��� ����
private:
	bool bJumping;
	bool bSprint;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Team")
		uint8 TeamID = 1;   //Team ID ���� �÷��̾�� 1 
};
