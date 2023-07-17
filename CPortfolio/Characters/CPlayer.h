#pragma once

#include "CoreMinimal.h"
#include "Characters/CCharacter.h"
#include "GenericTeamAgentInterface.h"   // 팀 ID 헤더
#include "CPlayer.generated.h"

UCLASS()
class CPORTFOLIO_API ACPlayer 
	: public ACCharacter
	, public IGenericTeamAgentInterface // 인터페이스 상속 

{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly)
		class UNiagaraComponent* NGDashComponent;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		class UCFeetComponent* FeetComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		class UCDashComponent* DashComponent;

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 컨트롤러 사용하지 않기 때문에 여기서 입력 처리
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//외부에 Team ID를 전달해주는 함수 
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
	void Begin_SubAction() override;		// Rifle은 Aim
	void End_SubAction() override;

private:
	void SkillQ() override;
	void SkillE() override;
	void SubSkill() override;

private:
	void Avoid() override;

private:
	void Dash();

public:
	void End_Dash();

private:
	bool bJumping;
	bool bSprint;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Team")
		uint8 TeamID = 1;			// Team ID 설정 플레이어는 1 

	UPROPERTY(EditAnywhere, Category = "Dash")
		float InitVel = 500.0f;		// 대쉬에 필요한 속도

	UPROPERTY(EditAnywhere, Category = "Dash")
		float InitDashCoolTime = 3.0f;	// 대쉬 쿨타임

	bool bUsedDash = false;
	float DashCoolTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
		class UNiagaraSystem* DashEffect;				// 대쉬 이펙트
};
