// Copyright (c) 2021, Radosław Paszkowski. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GASAbilitySystemComponent.generated.h"

UCLASS()
class HOTFIX_API UGASAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	bool bCharacterAbilitiesGiven = false;

	static UGASAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);

	virtual void AbilityLocalInputPressed(int32 InputID) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilities")
	FGameplayAbilitySpecHandle FindAbilitySpecHandleForClass(TSubclassOf<UGameplayAbility> AbilityClass, UObject* OptionalSourceObject=nullptr);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual bool TryActivateAbilityFromHandle(FGameplayAbilitySpecHandle InAbilityHandle, bool EndAbilityImmediately);

	virtual bool ShouldDoServerAbilityRPCBatch() const override { return true; }
};
