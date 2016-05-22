// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "AGameMode.h"
#include "APlayerController.h"
#include "ACharacter.h"
#include "APlayerState.h"
#include "AGameState.h"

AAGameMode::AAGameMode(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	/* Assign class types used by this mode */
	PlayerControllerClass = AAPlayerController::StaticClass();
	PlayerStateClass = AAPlayerState::StaticClass();
	GameStateClass = AAGameState::StaticClass();
	
}
