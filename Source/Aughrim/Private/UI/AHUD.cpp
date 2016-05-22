// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "AHUD.h"


AAHUD::AAHUD(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AAHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AAHUD::DrawCenterDot()
{

}

EHUDState AAHUD::GetCurrentState()
{
	return CurrentState;
}

void AAHUD::OnStateChanged_Implementation(EHUDState NewState)
{
	CurrentState = NewState;
}