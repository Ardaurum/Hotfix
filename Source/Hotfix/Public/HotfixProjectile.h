// Copyright (c) 2021, Radosław Paszkowski. All rights reserved

#pragma once

#include "CoreMinimal.h"

#include "GASAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "HotfixProjectile.generated.h"

UCLASS()
class HOTFIX_API AHotfixProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AHotfixProjectile();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile", Meta = (ExposeOnSpawn="true"))
	struct FGASEffectContainerSpec EffectContainer;

private:
	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	class USphereComponent* SphereCollider;

	UFUNCTION()
	virtual void OnOverlapBegin(class UPrimitiveComponent* NewComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
