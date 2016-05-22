// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "ABaseCharacter.h"
#include "ACharacterMovementComponent.h"

AABaseCharacter::AABaseCharacter(const class FObjectInitializer& ObjectInitializer)
	// Override the movement class to support multiple speeds (i.e. sprinting)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UACharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	Health = 100;

	TargetingSpeedModifier = 0.5f;
	SprintingSpeedModifier = 2.0f;

	// Don't collide with camera checks to keep 3rd person camera at position when players are standing behind us
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

float AABaseCharacter::GetMaxHealth() const
{
	// Retrieve default value of the health property assigned on instantiation
	return GetClass()->GetDefaultObject<AABaseCharacter>()->Health;
}

float AABaseCharacter::GetHealth() const
{
	return Health;
}

bool AABaseCharacter::IsAlive() const
{
	return Health > 0;
}

bool AABaseCharacter::IsSprinting() const
{
	if (!GetCharacterMovement())
	{
		return false;
	}
	// Perform fine-grain check
	return false;
}

void AABaseCharacter::SetSprinting(bool NewSprinting)
{
	// Handle 
}

float AABaseCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (Health <= 0.0f)
	{
		return 0.f;
	}

	// Base damage on gametype rules

	return 10.0f;
}

void AABaseCharacter::FellOutOfWorld(const class UDamageType& dmgType)
{
	// Kill player
}

