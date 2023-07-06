#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "CEnemy_AI.generated.h"

UCLASS()
class CPORTFOLIO_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()
	

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	//블프에서는 못하지만 C에서는 Team을 명시해줄 수 있음.
	//TeamID가 같으면 아군 다르면 적군, 255는 중립임.
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 2;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCAIStateComponent* AIState;

	UPROPERTY(EditAnywhere)
		class UCPatrolComponent* Patrol;

public:
	//팀 아이디 리턴 
	FORCEINLINE uint8 GetTeamID() { return TeamID; }

	//비헤이비어 트리 리턴
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }

public:
	ACEnemy_AI();

protected:
	virtual void BeginPlay() override;

};
