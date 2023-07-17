#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IDamage.h"
#include "CSkill2Assassin.generated.h"

UCLASS()
class CPORTFOLIO_API ACSkill2Assassin : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Cylinder;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Cone;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* BodyRed;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* BodyRedLine1;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* BodyRedLine2;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* BodyRedTop;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* BodyRedBottom;

	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Collision;

	UPROPERTY(VisibleDefaultsOnly)
		class UNiagaraComponent* NiagaraComponent;

	UPROPERTY(VisibleDefaultsOnly)
		class UAudioComponent* Sound;
	
//// Skill Data/////////////////////////////
public:
	UPROPERTY(VisibleDefaultsOnly, Category = "Data")
		TArray<FHitDatas> SkillEHitDatas;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "DataTable")
		class UDataTable* SkillEHitDataTable;

////////////////////////////////////////////

public:	
	ACSkill2Assassin();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void Detection();

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void SpawnComplete();

	UFUNCTION()
		void AttackTarget();	// FLatentActionInfo�� �ݹ��Լ��� ���Ƿ��� ����ȭ�� �Ǿ�� ��
	
	UFUNCTION()
		void ClearTarget();

	FORCEINLINE int32 GetNextUUID() { return nextUUID++; }

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
		TArray<class ACharacter*> Hitted;	// �ٴ���Ʈ ������ ���� Hitted ���� �迭

	UPROPERTY()
		TArray<class ACharacter*> Targets;	// Trace�� ������ Target ���� �迭

	UPROPERTY(EditAnywhere, Category = "Trace")
		float traceRadius = 1000.0f;		//<< Ÿ�� ���� ����

	UPROPERTY(EditAnywhere, Category = "Move")
		float MoveTime = 0.5f;				//<< MoveComponentTo������ �̵� �ð�


	float hitCount;
	int32 nextUUID = 0;

	FTimerHandle finishTimerHandle;			//<< ���Ͱ� �Ҹ�Ǳ� ���� FinishMission�� ȣ�����ִ� �ڵ鷯
};
