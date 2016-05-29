// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "AGameMode.h"
#include "ADamageType.h"
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
	
	return bWantsToRun;
}

void AABaseCharacter::SetSprinting(bool NewSprinting)
{
	bWantsToRun = NewSprinting;

	if (Role < ROLE_Authority)
	{
		ServerSetSprinting(NewSprinting);
	}
}

float AABaseCharacter::GetSprintingSpeedModifier() const
{
	return SprintingSpeedModifier;
}

void AABaseCharacter::ServerSetSprinting_Implementation(bool NewSprinting)
{
	SetSprinting(NewSprinting);
}

bool AABaseCharacter::ServerSetSprinting_Validate(bool NewSprinting)
{
	return true;
}

float AABaseCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (Health <= 0.0f)
	{
		return 0.f;
	}

	/* Modify based on gametype rules */
	AAGameMode* MyGameMode = Cast<AAGameMode>(GetWorld()->GetAuthGameMode());
	Damage = MyGameMode ? MyGameMode->ModifyDamage(Damage, this, DamageEvent, EventInstigator, DamageCauser) : Damage;

	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		Health -= ActualDamage;
		if (Health <= 0)
		{
			bool bCanDie = true;

			/* Check damagetype, always allow dying if the cast fails, otherwise check the property if player can die from damage type */
			if (DamageEvent.DamageTypeClass)
			{
				UADamageType* DmgType = Cast<UADamageType>(DamageEvent.DamageTypeClass->GetDefaultObject());
				bCanDie = (DmgType == nullptr || (DmgType && DmgType->GetCanDieFrom()));
			}

			if (bCanDie)
			{
				// Die!
			}
			else
			{
				/* Player cannot die from this damage type */
				Health = 1.0f;
			}
		}
		else
		{
			APawn* Pawn = EventInstigator ? EventInstigator->GetPawn() : nullptr;
			PlayHit(ActualDamage, DamageEvent, Pawn, DamageCauser, false);
		}

	}
	return 10.0f;
}

bool AABaseCharacter::CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const
{
	/* Check if char is already dying, destroyed or if we have authority */
	if (bIsDying ||
		IsPendingKill() ||
		Role != ROLE_Authority ||
		GetWorld()->GetAuthGameMode() == NULL ||
		GetWorld()->GetAuthGameMode()->GetMatchState() == MatchState::LeavingMap)
	{
		return false;
	}
	return true;
}

bool AABaseCharacter::Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	if (!CanDie(KillingDamage, DamageEvent, Killer, DamageCauser))
	{
		return false;
	}

	Health = FMath::Min(0.0f, Health);

	/* Fallback to default DamageType if none is specified */
	UDamageType const* const DamageType = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	Killer = GetDamageInstigator(Killer, *DamageType);

	/* Notify the gamemode we got killed for scoring and game over state */
	AController* KilledPlayer = Controller ? Controller : Cast<AController>(GetOwner());
	GetWorld().GetAuthGameMode<AAGameMode>()->Killed(Killer, KilledPlayer, this, DamageType);

	OnDeath(KillingDamage, DamageEvent, Killer ? Killer->GetPawn() : NULL, DamageCauser);
	return true;
}

void AABaseCharacter::OnDeath(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{

}

void AABaseCharacter::FellOutOfWorld(const class UDamageType& DmgType)
{
	Die(Health, FDamageEvent(DmgType.GetClass()), NULL, NULL);
}

void AABaseCharacter::SetRagdollPhysics()
{

}

void AABaseCharacter::PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser, bool bKilled)
{
	if (Role == ROLE_Authority)
	{
		ReplicatedHit(DamageTaken, DamageEvent, PawnInstigator, DamageCauser, bKilled);
	}

	if (GetNetMode() != NM_DedicatedServer)
	{
		if (bKilled && SoundDeath)
		{
			UGameplayStatics::SpawnSoundAttached(SoundDeath, RootComponent, NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);
		}
		else if (SoundTakeHit)
		{
			UGameplayStatics::SpawnSoundAttached(SoundTakeHit, RootComponent, NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);
		}
	}
}

void AABaseCharacter::ReplicatedHit(float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser, bool bKilled)
{

}

void AABaseCharacter::OnRep_LastTakeHitInfo()
{

}

void AABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Value is already updated locally, skip in replication step
	DOREPLIFETIME_CONDITION(AABaseCharacter, bWantsToRun, COND_SkipOwner);
}