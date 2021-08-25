// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerController.h"

#include "GameCharacter.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

void AGamePlayerController::BeginPlay()
{
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, nullptr, EMouseLockMode::LockAlways);
}

void AGamePlayerController::PlayerTick(float DeltaSeconds)
{
	Super::PlayerTick(DeltaSeconds);
	
	UWorld* World = GetWorld();
	if (!IsValid(World) || !IsValid(GetPawn()))
	{
		return;
	}

	FVector2D LookPos = FVector2D::ZeroVector;
	FVector RotationDir;
	FVector CameraDir;
	
	if (IsValid(InputComponent))
	{
		LookPos = FVector2D(InputComponent->GetAxisValue(TEXT("PadLookVert")), InputComponent->GetAxisValue(TEXT("PadLookHoriz")));
	}

	if (LookPos.SizeSquared() <= KINDA_SMALL_NUMBER)
	{
		const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(World);
		const float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(World);
		LookPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(World) * ViewportScale - ViewportSize / 2.0f;
		
		const FVector LookDir = UKismetMathLibrary::Conv_Vector2DToVector(LookPos.GetSafeNormal());
		RotationDir = FVector(-LookDir.Y, LookDir.X, .0f);
		CameraDir = FVector(.0f, LookDir.X, -LookDir.Y);

		if ((LookPos - LastMousePos).SizeSquared() <= KINDA_SMALL_NUMBER)
		{
			return;
		}
		LastMousePos = LookPos;
	}
	else
	{
		LookPos *= LookDeadZone;
		const FVector LookDir = UKismetMathLibrary::Conv_Vector2DToVector(LookPos.GetSafeNormal());
		RotationDir = FVector(-LookDir.X, LookDir.Y, .0f);
		CameraDir = FVector(.0f, LookDir.Y, -LookDir.X);
	}
	
	const FRotator NewRotation = UKismetMathLibrary::MakeRotFromX(RotationDir);

	GetPawn()->SetActorRotation(NewRotation);

	AGameCharacter* GameCharacter = Cast<AGameCharacter>(GetPawn());
	if (IsValid(GameCharacter))
	{
		UCameraComponent* CharacterCamera = GameCharacter->GetTopDownCameraComponent();
		const float LookDeadZoneSqr = LookDeadZone * LookDeadZone;
		const float LookForwardIntensity = FMath::Clamp(FMath::Abs(LookPos.SizeSquared()) / LookDeadZoneSqr, .0f, 1.0f);
		const FVector CameraTarget = CameraDir * LookForwardDistance * LookForwardIntensity;
		const FVector CurrentLocation = CharacterCamera->GetRelativeLocation();
		const FVector CameraOffset = FMath::VInterpTo(CurrentLocation, CameraTarget, DeltaSeconds, CameraInterpSpeed);

		CharacterCamera->SetRelativeLocation(CameraOffset);
	}
}

void AGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis(TEXT("MoveForward"), this, &AGamePlayerController::MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &AGamePlayerController::MoveRight);
	InputComponent->BindAxis(TEXT("PadLookVert"));
	InputComponent->BindAxis(TEXT("PadLookHoriz"));
}

void AGamePlayerController::MoveForward(const float Value)
{
	if (IsValid(GetPawn()))
	{
		GetPawn()->AddMovementInput(FVector::ForwardVector, Value);
	}
}

void AGamePlayerController::MoveRight(const float Value)
{
	if (IsValid(GetPawn()))
	{
		GetPawn()->AddMovementInput(FVector::RightVector, Value);
	}
}