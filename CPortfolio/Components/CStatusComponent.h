#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPORTFOLIO_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsCanMove() { return bCanMove; }
	FORCEINLINE bool IsFixedCamera() { return bFixedCamera; }

public:	
	UCStatusComponent();

protected:
	virtual void BeginPlay() override;

public:
	void Move();
	void Stop();
	void EnableFixedCamera() { bFixedCamera = true; }
	void DisableFixedCamera() { bFixedCamera = false; }

public:
	void EnableControlRotation();
	void DisableControlRotation();
	void BackupControlRotation();
	void RestoreControlRotation();

public:
	float GetHealth();

private:
	class ACharacter* Character;

private:
	bool bCanMove = true;
	bool bBackup_RotationYaw = false;
	bool bBackup_ToMovement = false;
	bool bFixedCamera = false;

};
