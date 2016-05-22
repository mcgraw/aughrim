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

	NumKills = 0;
	NumDeaths = 0;
}

void AAPlayerState::AddKill()
{
	NumKills++;
}

void AAPlayerState::AddDeath()
{
	NumDeaths++;
}

int32 AAPlayerState::GetKills() const
{
	return NumKills;
}

int32 AAPlayerState::GetDeaths() const
{
	return NumDeaths;
}
