#include "Weapons/CSkill2OneHand.h"
#include "Global.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"

ACSkill2OneHand::ACSkill2OneHand()
{
	PrimaryActorTick.bCanEverTick = true;

	//// 컴포넌트 생성
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<USphereComponent>(this, &Collision, "Collision", Scene);

	Collision->SetSphereRadius(500.0f);

	//PrimaryActorTick.TickInterval = 0.2f;	
}

void ACSkill2OneHand::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACSkill2OneHand::OnComponentBeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ACSkill2OneHand::OnComponentEndOverlap);

}

void ACSkill2OneHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (ACharacter* target : Targets)
	{
		FVector direction = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		float dist = target->GetDistanceTo(this);
		target->LaunchCharacter(-direction * (dist / 2), true, true);
	}
}

void ACSkill2OneHand::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	CheckNull(character);
	if (Targets.Find(character) < 0)
		Targets.AddUnique(character);

}

void ACSkill2OneHand::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(GetOwner() == OtherActor);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (Targets.Find(character) < 0)
		return;

	Targets.Remove(character);

}

