#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CFeetComponent.generated.h"

USTRUCT(BlueprintType)	// 자료형이 BP에서 나타나도록
struct FFeetData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
		FVector LeftDistance;		// x

	UPROPERTY(BlueprintReadOnly, Category = "Feet")
		FVector RightDistance;		// x

	UPROPERTY(BlueprintReadOnly, Category = "Feet")
		FVector PelvisDistance;		// z

	UPROPERTY(BlueprintReadOnly, Category = "Feet")
		FRotator LeftRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Feet")
		FRotator RightRotation;

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPORTFOLIO_API UCFeetComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Trace")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float InterpSpeed = 50;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float TraceDistance = 50;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float OffsetDistance = 5;

	UPROPERTY(EditAnywhere, Category = "Trace")
		FName LeftSocket = "Foot_L";

	UPROPERTY(EditAnywhere, Category = "Trace")
		FName RightSocket = "Foot_R";

public:
	// FORCEINLINE는 무조건 인라인화 시킴
	FORCEINLINE const FFeetData& GetData() { return Data; }		//  외부에서 받기 위한 함수 (인라인)


public:	
	UCFeetComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void Trace(FName InName, float& OutDistance, FRotator& OutRotation);

private:
	class ACharacter* OwnerCharacter;

	FFeetData Data;		// 외부에서 가져다 쓸 것임

};
