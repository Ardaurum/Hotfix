// Copyright (c) 2021, Radosław Paszkowski. All rights reserved


#include "HotfixProjectile.h"

#include "GameCharacter.h"
#include "GASAbilityTypes.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AHotfixProjectile::AHotfixProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0;
	ProjectileMovement->InitialSpeed = 4000.0f;

	SphereCollider = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	SphereCollider->SetSphereRadius(20.0f);
	SphereCollider->SetGenerateOverlapEvents(true);
	SphereCollider->SetupAttachment(RootComponent);
	SphereCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereCollider->bHiddenInGame = false;
	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AHotfixProjectile::OnOverlapBegin);

	bReplicates = true;

	NetUpdateFrequency = 20.0f;
}

void AHotfixProjectile::OnOverlapBegin(UPrimitiveComponent* NewComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetInstigator())
	{
		return;
	}

	AGameCharacter* OtherCharacter = Cast<AGameCharacter>(OtherActor);
	if (IsValid(OtherCharacter) && OtherCharacter->IsAlive())
	{
		EffectContainer.AddTargets(TArray<FGameplayAbilityTargetDataHandle>(), TArray<FHitResult>(), TArray<AActor*> {OtherActor});
		for (const FGameplayEffectSpecHandle& SpecHandle : EffectContainer.TargetGameplayEffectSpecs)
		{
			for (TSharedPtr<FGameplayAbilityTargetData> Data : EffectContainer.TargetData.Data)
			{
				Data->ApplyGameplayEffectSpec(*SpecHandle.Data.Get());
			}
		}
	}

	Destroy();
}
