// Copyright (c) 2021, Radosław Paszkowski. All rights reserved


#include "HotfixAttributeSet.h"

#include "Net/UnrealNetwork.h"

UHotfixAttributeSet::UHotfixAttributeSet()
{
	
}

void UHotfixAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UHotfixAttributeSet, Health, COND_None, REPNOTIFY_Always);
}

void UHotfixAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	//TODO: Handle effects like death
}

void UHotfixAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHotfixAttributeSet, Health, OldHealth);
}
