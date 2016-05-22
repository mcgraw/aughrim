// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "APlayerController.generated.h"

UENUM()
enum class EHUDMessage : uint8
{
	/* Weapon */
	Weapon_SlotTaken,

	/* Character */
	Character_EnergyRestored,

	/* Gamemode */
	Game_DeathmatchStart,
	Game_DeathmatchEnd,

	/* No category specified */
	None
};

UCLASS()
class AUGHRIM_API AAPlayerController : public APlayerController
{
	GENERATED_BODY()

	AAPlayerController(const FObjectInitializer& ObjectInitializer);

	// Flag to respawn or start spectating upon death
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	bool bRespawnImmediately;
	
	// Respawn or start spectating after dying
	virtual void UnFreeze() override;

};
