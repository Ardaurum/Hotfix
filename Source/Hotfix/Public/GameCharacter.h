// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TopDownCharacterMovement.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "HotfixAbilitySystemComponent.h"
#include "GameCharacter.generated.h"

class UHotfixAttributeSet;
class UCameraComponent;
class USpringArmComponent;
class AHotfixWeapon;

UCLASS()
class HOTFIX_API AGameCharacter : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGameCharacter();

	FORCEINLINE UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE UTopDownCharacterMovement* GetTopDownCharacterMovement() const { return TopDownMovement; }
	FORCEINLINE UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent; }
	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	bool IsAlive() const;
	float GetHealth() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UHotfixGameplayAbility>> CharacterAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	TSubclassOf<AHotfixWeapon> StartingWeapon;

	AHotfixWeapon* CurrentWeapon;

	virtual void PostInitializeComponents() override;

	virtual void AddCharacterAbilities();

	virtual void SetHealth(float Health);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupStartWeapons();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UTopDownCharacterMovement* TopDownMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	UHotfixAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	UHotfixAttributeSet* Attributes;

	bool bASCInputBound;

	void BindAbilitySystemInput();
};
