#include "AddOns/CGhostTrail.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

ACGhostTrail::ACGhostTrail()
{
	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &Mesh, "Mesh");
}

void ACGhostTrail::BeginPlay()
{
	Super::BeginPlay();
	
	Mesh->SetVisibility(false);

	// 머티리얼 인스턴스 가져옴
	UMaterialInstanceConstant* material;
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Materials/M_GhostTrail_Inst.M_GhostTrail_Inst'");

	// 다이나믹 머티리얼로 변경
	Material = UMaterialInstanceDynamic::Create(material, this);
	// 값 변경
	Material->SetVectorParameterValue("Color", Color);
	Material->SetScalarParameterValue("Exp", Exponent);

	//캐릭터의 SkeletalMesh를 빼와서 PoseableMesh의 SkeletalMesh를 설정해줌.
	Owner = Cast<ACharacter>(GetOwner());
	Mesh->SetSkeletalMesh(Owner->GetMesh()->SkeletalMesh);
	//Owner의 메쉬의 포즈를 복사 
	Mesh->CopyPoseFromSkeletalComponent(Owner->GetMesh());

	for (int32 i = 0; i < Owner->GetMesh()->SkeletalMesh->Materials.Num(); i++)		// 메시에 설정한 머티리얼 설정
		Mesh->SetMaterial(i, Material);


	//람다식을 사용한 델리게이트 정의 
	FTimerDelegate timerDelegate = FTimerDelegate::CreateLambda([=]()
		{
			if (Mesh->IsVisible() == false)
				Mesh->ToggleVisibility();

			//Interval 시간 마다 캐릭터의 Location, Rotation을 가져와서 업데이트
			float height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			SetActorLocation(Owner->GetActorLocation() - FVector(0, 0, height));
			SetActorRotation(Owner->GetActorRotation() + FRotator(0, -90, 0));

			Mesh->CopyPoseFromSkeletalComponent(Owner->GetMesh());
		});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, timerDelegate, Interval, true, StartDelay);
}


void ACGhostTrail::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//액터가 사라질때 호출되는 EndPlay에서는 TimerHandle를 비워줌
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

