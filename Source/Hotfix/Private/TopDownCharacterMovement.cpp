// Copyright (c) 2021, Radosław Paszkowski. All rights reserved


#include "TopDownCharacterMovement.h"

void UTopDownCharacterMovement::TickComponent(float DeltaTime, ELevelTick TickType,	FActorComponentTickFunction* ThisTickFunction)
{
	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(PawnOwner) || !IsValid(UpdatedComponent))
	{
		return;
	}

	const bool bIsSimulatingPhysics = UpdatedComponent->IsSimulatingPhysics();
	if (PawnOwner->GetLocalRole() == ROLE_Authority && bIsSimulatingPhysics && !PawnOwner->CheckStillInWorld())
	{
		return;
	}

	if (bIsSimulatingPhysics)
	{
		if (PawnOwner->GetLocalRole() == ROLE_AutonomousProxy && IsNetMode(NM_Client))
		{
			MarkForClientCameraUpdate();
		}

		Velocity = FVector::ZeroVector;
		return;
	}

	if (PawnOwner->GetLocalRole() > ROLE_SimulatedProxy)
	{
		// If we are a client we might have received an update from the server.
		const bool bIsClient = PawnOwner->GetLocalRole() == ROLE_AutonomousProxy && IsNetMode(NM_Client);
		if (bIsClient)
		{
			// TODO: Update data from server
		}

		if (PawnOwner->IsLocallyControlled())
		{
			const FVector InputVector = GetPendingInputVector();
			ControllerPawnMove(InputVector, DeltaTime);
			ConsumeInputVector();
		}
	}
	else if (PawnOwner->GetLocalRole() == ROLE_SimulatedProxy)
	{
		SimulatedTick(DeltaTime);
	}

	UpdateComponentVelocity();
}

float UTopDownCharacterMovement::GetMaxSpeed() const
{
	return MaxSpeed;
}

void UTopDownCharacterMovement::ControllerPawnMove(const FVector& Vector, float DeltaTime)
{
	Velocity = FMath::Clamp(Vector.Size(), .0f, 1.0f) * Vector.GetSafeNormal() * MaxSpeed;
	const FVector Delta = Velocity * DeltaTime;

	if (!Delta.IsNearlyZero(1e-6f))
	{
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();
		const FQuat Rotation = UpdatedComponent->GetComponentQuat();

		FHitResult Hit(1.f);
		SafeMoveUpdatedComponent(Delta, Rotation, true, Hit);

		if (Hit.IsValidBlockingHit())
		{
			HandleImpact(Hit, DeltaTime, Delta);
			SlideAlongSurface(Delta, 1.f-Hit.Time, Hit.Normal, Hit, true);
		}

		// Update velocity
		// We don't want position changes to vastly reverse our direction (which can happen due to penetration fixups etc)
		/*if (!bPositionCorrected)
		{
			const FVector NewLocation = UpdatedComponent->GetComponentLocation();
			Velocity = ((NewLocation - OldLocation) / DeltaTime);
		}*/
	}
}

void UTopDownCharacterMovement::SimulatedTick(float DeltaTime)
{
	//TODO: Implement simulated tick
}
