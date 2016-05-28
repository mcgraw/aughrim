// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Items/AUsableActor.h"
#include "APickupActor.generated.h"

UCLASS(ABSTRACT)
class AUGHRIM_API AAPickupActor : public AAUsableActor
{
	GENERATED_BODY()

	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* PickupSound;
	
	// NET

protected:

	AAPickupActor(const class FObjectInitializer& ObjectInitializer);

	// NET
	bool bIsActive;
	
	virtual void RespawnPickup();

	virtual void OnPickedUp();

	virtual void OnRespawned();

public:

	virtual void OnUsed(APawn* InstigatorPawn) override;

	/* Immediately spawn on begin play */
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	bool bStartActive;

	/* Will this item ever respawn */
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	bool bAllowRespawn;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	float RespawnDelay;

	/* Extra delay randomly applied on the respawn timer */
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	float RespawnDelayRange;

};
