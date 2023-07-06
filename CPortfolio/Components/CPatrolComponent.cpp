#include "Components/CPatrolComponent.h"
#include "Global.h"
#include "BehaviorTree/CPatrolActor.h"
#include "Components/SplineComponent.h"


UCPatrolComponent::UCPatrolComponent()
{

}

bool UCPatrolComponent::GetMoveTo(FVector& OutLocation,
	float& OutAcceptanceDistance)
{
	OutLocation = FVector::ZeroVector;
	OutAcceptanceDistance = 0;
	CheckFalseResult(IsValidPatrol(), false);

	OutLocation = Patrol->GetSpline()->GetLocationAtSplinePoint(Index,
		ESplineCoordinateSpace::World);
	OutAcceptanceDistance = AcceptanceDistance;

	return true;
}

void UCPatrolComponent::Update()
{
	CheckFalse(IsValidPatrol());

	int32 count = Patrol->GetSpline()->GetNumberOfSplinePoints();
	if (bReverse)
	{
		if (Index > 0)
		{
			Index--;

			return;
		}

		if (Patrol->GetSpline()->IsClosedLoop())
		{
			Index = count - 1;

			return;
		}

		Index = 1;
		bReverse = false;

		return;
	}

	if (Index < count - 1)
	{
		Index++;

		return;
	}

	if (Patrol->GetSpline()->IsClosedLoop())
	{
		Index = 0;

		return;
	}

	Index = count - 2;
	bReverse = true;
}