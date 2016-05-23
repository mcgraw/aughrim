// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "AGameMode.generated.h"

UCLASS()
class AUGHRIM_API AAGameMode : public AGameMode
{
	GENERATED_BODY()
	
protected:

	AAGameMode(const class FObjectInitializer& ObjectInitializer);

	virtual void PreInitializeComponents() override;

	virtual void InitGameState();

	virtual void DefaultTimer();

	virtual void StartMatch();

	virtual void SpawnDefaultInventory(APawn* PlayerPawn);

	virtual void SetPlayerDefaults(APawn* PlayerPawn) override;

	/* Handle for efficient management of DefaultTimer timer */
	FTimerHandle TimerHandle_DefaultTimer;

	float BotSpawnInterval;

	/* Called once on every new player that enters the gamemode */
	virtual FString InitNewPlayer(class APlayerController* NewPlayerController, const TSharedPtr<const FUniqueNetId>& UniqueId, const FString& Options, const FString& Portal = TEXT(""));

	// Bot spawn handling


	/************************************************************************/
	/* Player Spawning                                                      */
	/************************************************************************/

	/* Always pick a random location */
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;

	virtual bool IsSpawnpointAllowed(APlayerStart* SpawnPoint, AController* Controller);

	virtual bool IsSpawnpointPreferred(APlayerStart* SpawnPoint, AController* Controller);

	/* Return default pawn class for given controller */
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	/************************************************************************/
	/* Damage & Killing                                                     */
	/************************************************************************/

public: 

	virtual void Killed(AController* Killer, AController* VictimPlayer, APawn* VictimPawn, const UDamageType* DamageType);

	virtual bool CanDealDamage(class ASPlayerState* DamageCauser, class ASPlayerState* DamagedPlayer) const;

	virtual float ModifyDamage(float Damage, AActor* DamagedActor, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) const;

	/* Default weapons to spawn with */
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TArray<TSubclassOf<class AAWeapon>> DefaultInventoryClasses;
	
};
