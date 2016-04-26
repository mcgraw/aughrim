// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Aughrim.h"
#include "AughrimGameMode.h"
#include "AughrimHUD.h"
#include "AughrimCharacter.h"

AAughrimGameMode::AAughrimGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AAughrimHUD::StaticClass();
}
