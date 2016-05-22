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

	/* End the match */
	void FinishMatch();

	/* Spawn the player */
	virtual void RestartPlayer(class AController* NewPlayer) override;
		
	
};
