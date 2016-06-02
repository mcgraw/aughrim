// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "APlayerState.h"


AAPlayerState::AAPlayerState(const class FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{

}

void AAPlayerState::Reset()
{
	Super::Reset();

	NumPlayerKills = 0;
	NumBotKills = 0;
	NumDeaths = 0;
}

void AAPlayerState::AddPlayerKill()
{
	NumPlayerKills++;
}

void AAPlayerState::AddBotKill()
{
	NumBotKills++;
}

void AAPlayerState::AddDeath()
{
	NumDeaths++;
}

int32 AAPlayerState::GetPlayerKills() const
{ 
	return NumPlayerKills;
}

int32 AAPlayerState::GetBotKills() const
{
	return NumBotKills;
}

int32 AAPlayerState::GetDeaths() const
{
	return NumDeaths;
}

void AAPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAPlayerState, NumPlayerKills);
	DOREPLIFETIME(AAPlayerState, NumBotKills);
	DOREPLIFETIME(AAPlayerState, NumDeaths);

}