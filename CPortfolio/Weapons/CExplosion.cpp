#include "Weapons/CExplosion.h"
#include "Global.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Sound/SoundWave.h"


ACExplosion::ACExplosion()
{
	CHelpers::CreateComponent<UNiagaraComponent>(this, &NiagaraComponent, "Niagara");

	UNiagaraSystem* ngSystem;
	CHelpers::GetAsset<UNiagaraSystem>(&ngSystem, "NiagaraSystem'/Game/Effects/NG_RifleSkill1_Explosion.NG_RifleSkill1_Explosion'");
	NiagaraComponent->SetAsset(ngSystem);
	
	NiagaraComponent->bAutoActivate = true;

	CHelpers::GetAsset<USoundWave>(&ExplosionSound, "SoundWave'/Game/Sounds/RifleExplosion.RifleExplosion'");

	InitialLifeSpan = 10.0f;
}

void ACExplosion::BeginPlay()
{
	Super::BeginPlay();
	

	if (!!ExplosionSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());

}

