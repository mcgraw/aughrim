// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "APlayerController.h"


AAPlayerController::AAPlayerController(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AAPlayerController::UnFreeze()
{
	Super::UnFreeze();
}
