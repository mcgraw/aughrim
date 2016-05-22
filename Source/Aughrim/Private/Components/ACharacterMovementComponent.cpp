// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "ACharacterMovementComponent.h"
#include "ABaseCharacter.h"

float UACharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	const AABaseCharacter* CharOwner = Cast<AABaseCharacter>(PawnOwner);
	if (CharOwner)
	{
		// Slow down during targeting or crouching
	}
	return MaxSpeed;
}
