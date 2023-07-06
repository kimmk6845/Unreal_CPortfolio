#include "Weapons/CMagazine.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"

ACMagazine::ACMagazine()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh", Root);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh_Empty, "Mesh_Empty", Root);

	Mesh->SetCollisionProfileName("Magazine");
	Mesh_Empty->SetCollisionProfileName("Magazine");

	UStaticMesh* mesh;
	CHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Game/WeaponMesh/Rifle/SM_AR4_Mag.SM_AR4_Mag'");
	Mesh->SetStaticMesh(mesh);

	CHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Game/WeaponMesh/Rifle/SM_AR4_Mag_Empty.SM_AR4_Mag_Empty'");
	Mesh_Empty->SetStaticMesh(mesh);
}

void ACMagazine::BeginPlay()
{
	Super::BeginPlay();

	if (bEject == false)
	{
		Mesh_Empty->SetVisibility(false);
		Mesh->SetVisibility(true);
	}

}


void ACMagazine::Eject()
{
	bEject = true;

	Mesh->SetVisibility(false);		// ²ËÂù ÅºÃ¢Àº ¼û±è
	Mesh_Empty->SetSimulatePhysics(true);
}