// Copyright (c) 2021, Radosław Paszkowski. All rights reserved

#include "HotfixWeapon.h"
#include "HotfixAbilitySystemComponent.h"
#include "HotfixGameplayAbility.h"
#include "GameCharacter.h"
#include "Net/UnrealNetwork.h"

AHotfixWeapon::AHotfixWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	bNetUseOwnerRelevancy = true;
	NetUpdateFrequency = 20.0f;
}

UAbilitySystemComponent* AHotfixWeapon::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AHotfixWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AHotfixWeapon, OwningCharacter, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AHotfixWeapon, Ammo, COND_OwnerOnly);
}

void AHotfixWeapon::SetOwningCharacter(AGameCharacter* InOwningCharacter)
{
	OwningCharacter = InOwningCharacter;
	if (OwningCharacter)
	{
		AbilitySystemComponent = Cast<UHotfixAbilitySystemComponent>(OwningCharacter->GetAbilitySystemComponent());
		SetOwner(OwningCharacter);
		AttachToComponent(OwningCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
	else
	{
		AbilitySystemComponent = nullptr;
		SetOwner(nullptr);
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void AHotfixWeapon::AddAbilities()
{
	if (!IsValid(OwningCharacter) || !OwningCharacter->GetAbilitySystemComponent())
	{
		return;
	}

	UHotfixAbilitySystemComponent* AbilitySystem = Cast<UHotfixAbilitySystemComponent>(OwningCharacter->GetAbilitySystemComponent());

	if (!AbilitySystem)
	{
		UE_LOG(LogTemp, Error, TEXT("%s %s ASC is null"), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	for (TSubclassOf<UHotfixGameplayAbility>& Ability : Abilities)
	{
		AbilitySpecHandles.Add(AbilitySystem->GiveAbility(
			FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this)));
	}
}

void AHotfixWeapon::RemoveAbilities()
{
	if (!IsValid(OwningCharacter) || !OwningCharacter->GetAbilitySystemComponent())
	{
		return;
	}

	UHotfixAbilitySystemComponent* AbilitySystem = Cast<UHotfixAbilitySystemComponent>(OwningCharacter->GetAbilitySystemComponent());

	if (!AbilitySystem)
	{
		return;
	}

	// Remove abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	for (FGameplayAbilitySpecHandle& SpecHandle : AbilitySpecHandles)
	{
		AbilitySystem->ClearAbility(SpecHandle);
	}
}

int32 AHotfixWeapon::GetAmmo() const
{
	return Ammo;
}

void AHotfixWeapon::SetAmmo(int32 NewAmmo)
{
	Ammo = FMath::Max(NewAmmo, 0);
}

bool AHotfixWeapon::HasInfiniteAmmo() const
{
	return bInfiniteAmmo;
}