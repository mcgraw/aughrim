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
	UE_LOG(LogTemp, Warning, TEXT("TAKE DAMAGE"))
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
				Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
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
	return ActualDamage;
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

	UE_LOG(LogTemp, Warning, TEXT("Should die"))

	Health = FMath::Min(0.0f, Health);

	/* Fallback to default DamageType if none is specified */
	UDamageType const* const DamageType = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	Killer = GetDamageInstigator(Killer, *DamageType);

	/* Notify the gamemode we got killed for scoring and game over state */
	AController* KilledPlayer = Controller ? Controller : Cast<AController>(GetOwner());
	GetWorld()->GetAuthGameMode<AAGameMode>()->Killed(Killer, KilledPlayer, this, DamageType);

	OnDeath(KillingDamage, DamageEvent, Killer ? Killer->GetPawn() : NULL, DamageCauser);
	return true;
}

void AABaseCharacter::OnDeath(float KillingDamage, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	if (bIsDying)
	{
		return;
	}

	bReplicateMovement = false;
	bTearOff = true;
	bIsDying = true;

	PlayHit(KillingDamage, DamageEvent, PawnInstigator, DamageCauser, true);

	DetachFromControllerPendingDestroy();

	/* Disable all collision on capsule */
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	USkeletalMeshComponent* Mesh3P = GetMesh();
	if (Mesh3P)
	{
		Mesh3P->SetCollisionProfileName(TEXT("Ragdoll"));
	}
	SetActorEnableCollision(true);

	SetRagdollPhysics();

	/* Apply physics impulse on the bone of the enemy skeleton mesh we hit */
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent PointDmg = *((FPointDamageEvent*)(&DamageEvent));
		{
			Mesh3P->AddImpulseAtLocation(PointDmg.ShotDirection * 12000, PointDmg.HitInfo.ImpactPoint, PointDmg.HitInfo.BoneName);
		}
	}
	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent RadialDmg = *((FRadialDamageEvent*)(&DamageEvent));
		{
			Mesh3P->AddRadialImpulse(RadialDmg.Origin, RadialDmg.Params.GetMaxRadius(), 10000 /* RadialDmg.DamageTypeClass->DamageImpuse */, ERadialImpulseFalloff::RIF_Linear);
		}
	}
}

void AABaseCharacter::FellOutOfWorld(const class UDamageType& DmgType)
{
	Die(Health, FDamageEvent(DmgType.GetClass()), NULL, NULL);
}

void AABaseCharacter::SetRagdollPhysics()
{
	bool bInRagdoll = false;
	USkeletalMeshComponent* Mesh3P = GetMesh();

	if (IsPendingKill())
	{
		bInRagdoll = false;
	}
	else if (!Mesh3P || !Mesh3P->GetPhysicsAsset())
	{
		bInRagdoll = false;
	}
	else
	{
		Mesh3P->SetAllBodiesSimulatePhysics(true);
		Mesh3P->SetSimulatePhysics(true);
		Mesh3P->WakeAllRigidBodies();
		Mesh3P->bBlendPhysics = true;

		bInRagdoll = true;
	}

	UCharacterMovementComponent* CharacterComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (CharacterComp)
	{
		CharacterComp->StopMovementImmediately();
		CharacterComp->DisableMovement();
		CharacterComp->SetComponentTickEnabled(false);
	}

	if (!bInRagdoll)
	{
		// Immediately hide the pawn
		TurnOff();
		SetActorHiddenInGame(true);
		SetLifeSpan(1.0f);
	}
	else
	{
		SetLifeSpan(10.0f);
	}
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
	const float TimeoutTime = GetWorld()->GetTimeSeconds() + 0.5f;

	FDamageEvent const& LastDamageEvent = LastTakeHitInfo.GetDamageEvent();
	if (PawnInstigator == LastTakeHitInfo.PawnInstigator.Get() && LastDamageEvent.DamageTypeClass == LastTakeHitInfo.DamageTypeClass)
	{
		// Same frame damage
		if (bKilled && LastTakeHitInfo.bKilled)
		{
			// Redundant death take hit, ignore it
			return;
		}

		DamageTaken += LastTakeHitInfo.ActualDamage;
	}

	LastTakeHitInfo.ActualDamage = DamageTaken;
	LastTakeHitInfo.PawnInstigator = Cast<AABaseCharacter>(PawnInstigator);
	LastTakeHitInfo.DamageCauser = DamageCauser;
	LastTakeHitInfo.SetDamageEvent(DamageEvent);
	LastTakeHitInfo.bKilled = bKilled;
	LastTakeHitInfo.EnsureReplication();
}

void AABaseCharacter::OnRep_LastTakeHitInfo()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_LastTakeHitInfo"))

	if (LastTakeHitInfo.bKilled)
	{
		OnDeath(LastTakeHitInfo.ActualDamage, LastTakeHitInfo.GetDamageEvent(), LastTakeHitInfo.PawnInstigator.Get(), LastTakeHitInfo.DamageCauser.Get());
	}
	else
	{
		PlayHit(LastTakeHitInfo.ActualDamage, LastTakeHitInfo.GetDamageEvent(), LastTakeHitInfo.PawnInstigator.Get(), LastTakeHitInfo.DamageCauser.Get(), LastTakeHitInfo.bKilled);
	}
}

void AABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Value is already updated locally, skip in replication step
	DOREPLIFETIME_CONDITION(AABaseCharacter, bWantsToRun, COND_SkipOwner);

	// Replicate to every client, no special condition required
	DOREPLIFETIME(AABaseCharacter, Health);
	DOREPLIFETIME(AABaseCharacter, LastTakeHitInfo);
}