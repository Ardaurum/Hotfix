// Copyright (c) 2021, Radosław Paszkowski. All rights reserved

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "GameFramework/Actor.h"
#include "HotfixWeapon.generated.h"

class UHotfixAbilitySystemComponent;
class AGameCharacter;
class UHotfixGameplayAbility;

UCLASS()
class HOTFIX_API AHotfixWeapon : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	AHotfixWeapon();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SetOwningCharacter(AGameCharacter* InOwningCharacter);
	
	virtual void AddAbilities();

	virtual void RemoveAbilities();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual int32 GetAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void SetAmmo(int32 NewAmmo);

	virtual bool HasInfiniteAmmo() const;

protected:
	UPROPERTY()
	UHotfixAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Replicated, Category = "Weapon")
	int32 Ammo;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
	bool bInfiniteAmmo;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Weapon")
	AGameCharacter* OwningCharacter;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UHotfixGameplayAbility>> Abilities;

	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};
