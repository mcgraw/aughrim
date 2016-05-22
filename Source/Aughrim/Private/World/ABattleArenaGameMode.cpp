// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "ABattleArenaGameMode.h"


AABattleArenaGameMode::AABattleArenaGameMode(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AABattleArenaGameMode::CheckMatchEnd()
{

}

void AABattleArenaGameMode::FinishMatch()
{

}

void AABattleArenaGameMode::RestartPlayer(class AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}
