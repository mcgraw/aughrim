// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "AWeaponInstant.h"

AAWeaponInstant::AAWeaponInstant(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HitDamage = 26;
	WeaponRange = 15000;

	AllowedViewDotHitDir = -1.0f;
	ClientSideHitLeeway = 200.0f;
	MinimumProjectileSpawnDistance = 800;
	TracerRoundInterval = 3;
}

void AAWeaponInstant::SimulateInstantHit(const FVector& ImpactPoint)
{

}

void AAWeaponInstant::SpawnImpactEffects(const FHitResult& Impact)
{

}

void AAWeaponInstant::SpawnTrailEffects(const FVector& EndPoint)
{

}

void AAWeaponInstant::FireWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("FireWeapon"));

	const FVector AimDir = GetAdjustedAim();
	const FVector CameraPos = GetCameraDamageStartLocation(AimDir);
	const FVector EndPos = CameraPos + (AimDir * WeaponRange);

	/* Check for impact by tracing from the camera position */
	FHitResult Impact = WeaponTrace(CameraPos, EndPos);

	const FVector MuzzleOrigin = GetMuzzleLocation();

	FVector AdjustedAimDir = AimDir;
	if (Impact.bBlockingHit)
	{
		/* Adjust the shoot direction to hit at the crosshair */
		AdjustedAimDir = (Impact.ImpactPoint - MuzzleOrigin).GetSafeNormal();

		/* Re-trace with the new aim direction coming out of the weapon muzzle */
		Impact = WeaponTrace(MuzzleOrigin, MuzzleOrigin + (AdjustedAimDir * WeaponRange));
	}
	else
	{
		/* Use the maximum distance as the adjust direction */
		Impact.ImpactPoint = FVector_NetQuantize(EndPos);
	}

	ProcessInstantHit(Impact, MuzzleOrigin, AdjustedAimDir);
}

void AAWeaponInstant::DealDamage(const FHitResult& Impact, const FVector& ShootDir)
{

}

bool AAWeaponInstant::ShouldDealDamage(AActor* TestActor) const
{
	return false;
}

void AAWeaponInstant::ProcessInstantHit(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDir)
{

	// Process a confirmed hit
	ProcessInstantHitConfirmed(Impact, Origin, ShootDir);
}

void AAWeaponInstant::ProcessInstantHitConfirmed(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDir)
{
	HitImpactNotify = Impact.ImpactPoint;
}
