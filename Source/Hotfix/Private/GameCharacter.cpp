// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "TopDownCharacterMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "HotfixWeapon.h"
#include "HotfixGameplayAbility.h"
#include "HotfixAttributeSet.h"

AGameCharacter::AGameCharacter()
{
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
 	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	TopDownMovement = CreateDefaultSubobject<UTopDownCharacterMovement>(TEXT("CharacterMovement"));
	TopDownMovement->bConstrainToPlane = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetupAttachment(CapsuleComponent);

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	AbilitySystemComponent = CreateDefaultSubobject<UHotfixAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	Attributes = CreateDefaultSubobject<UHotfixAttributeSet>("Attributes");
}

bool AGameCharacter::IsAlive() const
{
	return GetHealth() >= .0f;
}

float AGameCharacter::GetHealth() const
{
	if (IsValid(Attributes))
	{
		return Attributes->GetHealth();
	}

	return .0f;
}

void AGameCharacter::AddCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !IsValid(AbilitySystemComponent) || AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UHotfixGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void AGameCharacter::SetHealth(float Health)
{
	if (IsValid(Attributes))
	{
		Attributes->SetHealth(Health);
	}
}

void AGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	BindAbilitySystemInput();
}

void AGameCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AddCharacterAbilities();
}

void AGameCharacter::SetupStartWeapons()
{
	if (GetLocalRole() < ROLE_Authority || !StartingWeapon)
	{
		return;
	}

	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	CurrentWeapon = GetWorld()->SpawnActor<AHotfixWeapon>(StartingWeapon, FTransform::Identity, SpawnParams);
	CurrentWeapon->SetOwningCharacter(this);
	CurrentWeapon->AddAbilities();
}

void AGameCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetupStartWeapons();
}

void AGameCharacter::BindAbilitySystemInput()
{
	if (!bASCInputBound && IsValid(AbilitySystemComponent) && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), FString("EHotfixAbilityInputID"), static_cast<int32>(EHotfixAbilityInputID::Confirm), static_cast<int32>(EHotfixAbilityInputID::Cancel)));

		bASCInputBound = true;
	}
}
