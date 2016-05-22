// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "ACharacter.h"

AACharacter::AACharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AACharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AACharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AACharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}

void AACharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AACharacter::PawnClientRestart()
{
	Super::PawnClientRestart();
}

void AACharacter::MoveForward(float Val)
{
	if (Controller && Val != 0.f)
	{
		const FRotator Rotation = GetActorRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Val);
	}
}

void AACharacter::MoveRight(float Val)
{
	if (Val != 0.f)
	{
		const FRotator Rotation = GetActorRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Val);
	}
}

void AACharacter::OnStartJump()
{
	bPressedJump = true;
	SetIsJumping(true);
}

void AACharacter::OnStopJump()
{
	bPressedJump = false;
}

void AACharacter::OnStartSprinting()
{
	SetSprinting(true);
}

void AACharacter::OnStopSprinting()
{
	SetSprinting(false);
}

void AACharacter::SetSprinting(bool NewSprinting)
{
	if (bWantsToRun)
	{
		StopWeaponFire();
	}
	Super::SetSprinting(NewSprinting);
}

bool AACharacter::IsInitiatedJump() const
{
	return bIsJumping;
}

void AACharacter::SetIsJumping(bool NewJumping)
{
	bIsJumping = NewJumping;
}

void AACharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode /*= 0*/)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (PrevMovementMode == EMovementMode::MOVE_Falling && GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling)
	{
		SetIsJumping(false);
	}
}

void AACharacter::OnToggleCarryActor()
{
	//CarriedObjectComp->Pickup()
}

void AACharacter::OnReload()
{

}

void AACharacter::OnStartFire()
{

}

void AACharacter::OnStopFire()
{

}

void AACharacter::StartWeaponFire()
{

}

void AACharacter::StopWeaponFire()
{

}
