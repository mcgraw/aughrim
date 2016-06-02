// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "World/AGameMode.h"
#include "ABattleArenaGameMode.generated.h"

UCLASS(ABSTRACT)
class AUGHRIM_API AABattleArenaGameMode : public AAGameMode
{
	GENERATED_BODY()

	AABattleArenaGameMode(const class FObjectInitializer& ObjectInitializer);

	/* End match when last man is standing */
	void CheckMatchEnd();

	/* Begin a new match */
	void BeginNextMatch();

	/* End the match */
	void FinishMatch();

	/* Spawn the player */
	virtual void RestartPlayer(class AController* NewPlayer) override;
		
	virtual void Killed(AController* Killer, AController* VictimPlayer, APawn* VictimPawn, const UDamageType* DamageType) override;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	int32 MaxBattleRounds;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	int32 CurrentBattleRound;
};
