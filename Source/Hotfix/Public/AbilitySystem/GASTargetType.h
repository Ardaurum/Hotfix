// Copyright (c) 2021, Radosław Paszkowski. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GASAbilityTypes.h"
#include "GASTargetType.generated.h"

class AGameCharacter;

UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class HOTFIX_API UGASTargetType : public UObject
{
	GENERATED_BODY()

public:
	UGASTargetType() {}

	UFUNCTION(BlueprintNativeEvent)
	void GetTargets(AGameCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FGameplayAbilityTargetDataHandle>& OutTargetData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
	virtual void GetTargets_Implementation(AGameCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FGameplayAbilityTargetDataHandle>& OutTargetData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};

UCLASS(NotBlueprintable)
class HOTFIX_API UGASTargetType_UseOwner : public UGASTargetType
{
	GENERATED_BODY()

public:
	UGASTargetType_UseOwner() {}

	virtual void GetTargets_Implementation(AGameCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FGameplayAbilityTargetDataHandle>& OutTargetData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

UCLASS(NotBlueprintable)
class HOTFIX_API UGASTargetType_UseEventData : public UGASTargetType
{
	GENERATED_BODY()

public:
	UGASTargetType_UseEventData() {}

	virtual void GetTargets_Implementation(AGameCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FGameplayAbilityTargetDataHandle>& OutTargetData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};