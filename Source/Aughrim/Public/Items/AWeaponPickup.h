// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Items/APickupActor.h"
#include "AWeaponPickup.generated.h"

UCLASS(ABSTRACT)
class AUGHRIM_API AAWeaponPickup : public AAPickupActor
{
	GENERATED_BODY()

	AAWeaponPickup(const class FObjectInitializer& ObjectInitializer);

public:

	/* Class to add to inventory when picked up */
	UPROPERTY(EditDefaultsOnly, Category = "WeaponClass")
	TSubclassOf<AAWeapon> WeaponClass;

	virtual void OnUsed(APawn* InstigatorPawn) override;
	
};
