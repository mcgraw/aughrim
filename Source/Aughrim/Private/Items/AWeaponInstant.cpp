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

}

void AAWeaponInstant::ProcessInstantHitConfirmed(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDir)
{

}
