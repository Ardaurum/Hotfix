// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"

UCLASS()
class HOTFIX_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	
private:
	void MoveForward(const float Value);
	void MoveRight(const float Value);

	UPROPERTY(EditAnywhere, Category = Camera)
	float LookForwardDistance = 20.0f;

	UPROPERTY(EditAnywhere, Category = Camera)
	float LookDeadZone = 10.0f;

	UPROPERTY(EditAnywhere, Category = Camera)
	float CameraInterpSpeed = 0.5f;

	FVector2D LastMousePos;
};
