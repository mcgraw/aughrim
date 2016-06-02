// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "ACharacter.h"
#include "APlayerState.h"
#include "ABattleArenaGameMode.h"

AABattleArenaGameMode::AABattleArenaGameMode(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentBattleRound = 0;
	MaxBattleRounds = 3;
}

void AABattleArenaGameMode::CheckMatchEnd()
{
	bool bHasDeadPlayer = false;
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
	{
		AACharacter* MyPawn = Cast<AACharacter>(*It);
		if (MyPawn && !MyPawn->IsAlive())
		{
			AAPlayerState* PS = Cast<AAPlayerState>(MyPawn->PlayerState);
			if (PS)
			{
				if (!PS->bIsABot)
				{
					bHasDeadPlayer = true;
					break;
				}
			}
		}
	}

	if (bHasDeadPlayer)
	{
		CurrentBattleRound++;
		if (CurrentBattleRound == MaxBattleRounds)
		{
			FinishMatch();
		}
		else
		{
			BeginNextMatch();
		}
	}
}

void AABattleArenaGameMode::BeginNextMatch()
{
	// Move to the summary screen
}

void AABattleArenaGameMode::FinishMatch()
{
	// Complete match
}

void AABattleArenaGameMode::RestartPlayer(class AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}

void AABattleArenaGameMode::Killed(AController* Killer, AController* VictimPlayer, APawn* VictimPawn, const UDamageType* DamageType)
{
	AAPlayerState* KillerPS = Killer ? Cast<AAPlayerState>(Killer->PlayerState) : NULL;
	AAPlayerState* VictimPS = VictimPlayer ? Cast<AAPlayerState>(VictimPlayer->PlayerState) : NULL;

	if (KillerPS && KillerPS != VictimPS && !KillerPS->bIsABot)
	{
		KillerPS->AddPlayerKill();
	}

	if (VictimPS && !VictimPS->bIsABot)
	{
		VictimPS->AddDeath();
	}

	CheckMatchEnd();
}
