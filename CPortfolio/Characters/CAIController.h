#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

UCLASS()
class CPORTFOLIO_API ACAIController : public AAIController
{
	GENERATED_BODY()

private:
	//���� �ν��ϴ� Perception ������Ʈ
	UPROPERTY(VisibleDefaultsOnly) //Component�� VisibleDefaultsOnly
		class UAIPerceptionComponent* Perception;

public:
	ACAIController();

protected:
	virtual void BeginPlay() override;

protected:
	//���ǰ� �ɶ� ȣ�� �Ǵ� OnPossess
	virtual void OnPossess(APawn* InPawn) override;
	//���ǰ� �����ɶ� ȣ��Ǵ� OnUnPossess
	virtual void OnUnPossess() override;

public:
	//OnPerceptionUpdate ��������Ʈ�� ������ �Լ� 
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:
	class ACEnemy_AI* Enemy;
	//Perception ��� �þ� Sight
	class UAISenseConfig_Sight* Sight;
	class UCAIStateComponent* AIState;

};
