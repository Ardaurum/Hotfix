// Copyright (c) 2021, Radosław Paszkowski. All rights reserved


#include "AbilitySystem/GASAbilityTypes.h"

bool FGASEffectContainerSpec::HasValidEffects() const
{
	return TargetGameplayEffectSpecs.Num() > 0;
}

bool FGASEffectContainerSpec::HasValidTargets() const
{
	return TargetData.Num() > 0;
}

void FGASEffectContainerSpec::AddTargets(const TArray<FGameplayAbilityTargetDataHandle>& InTargetData, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors)
{
	for (const FGameplayAbilityTargetDataHandle& Handle : InTargetData)
	{
		TargetData.Append(Handle);
	}

	for (const FHitResult& HitResult : HitResults)
	{
		FGameplayAbilityTargetData_SingleTargetHit* NewData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
		TargetData.Add(NewData);
	}

	if (TargetActors.Num() > 0)
	{
		FGameplayAbilityTargetData_ActorArray* NewData = new FGameplayAbilityTargetData_ActorArray();
		NewData->TargetActorArray.Append(TargetActors);
		TargetData.Add(NewData);
	}
}

void FGASEffectContainerSpec::ClearTargets()
{
	TargetData.Clear();
}
