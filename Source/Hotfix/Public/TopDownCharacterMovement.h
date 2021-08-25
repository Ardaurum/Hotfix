// Copyright (c) 2021, Radosław Paszkowski. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "TopDownCharacterMovement.generated.h"

/**
 * 
 */
UCLASS()
class HOTFIX_API UTopDownCharacterMovement : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual float GetMaxSpeed() const override;

private:
	void ControllerPawnMove(const FVector& Vector, float DeltaTime);
	void SimulatedTick(float DeltaTime);

	UPROPERTY(EditAnywhere)
	float MaxSpeed = 100.0f;
	
};
