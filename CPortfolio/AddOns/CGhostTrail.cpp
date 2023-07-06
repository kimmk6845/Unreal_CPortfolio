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

	// ��Ƽ���� �ν��Ͻ� ������
	UMaterialInstanceConstant* material;
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Materials/M_GhostTrail_Inst.M_GhostTrail_Inst'");

	// ���̳��� ��Ƽ����� ����
	Material = UMaterialInstanceDynamic::Create(material, this);
	// �� ����
	Material->SetVectorParameterValue("Color", Color);
	Material->SetScalarParameterValue("Exp", Exponent);

	//ĳ������ SkeletalMesh�� ���ͼ� PoseableMesh�� SkeletalMesh�� ��������.
	Owner = Cast<ACharacter>(GetOwner());
	Mesh->SetSkeletalMesh(Owner->GetMesh()->SkeletalMesh);
	//Owner�� �޽��� ��� ���� 
	Mesh->CopyPoseFromSkeletalComponent(Owner->GetMesh());

	for (int32 i = 0; i < Owner->GetMesh()->SkeletalMesh->Materials.Num(); i++)		// �޽ÿ� ������ ��Ƽ���� ����
		Mesh->SetMaterial(i, Material);


	//���ٽ��� ����� ��������Ʈ ���� 
	FTimerDelegate timerDelegate = FTimerDelegate::CreateLambda([=]()
		{
			if (Mesh->IsVisible() == false)
				Mesh->ToggleVisibility();

			//Interval �ð� ���� ĳ������ Location, Rotation�� �����ͼ� ������Ʈ
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

	//���Ͱ� ������� ȣ��Ǵ� EndPlay������ TimerHandle�� �����
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

