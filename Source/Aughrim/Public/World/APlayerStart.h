// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerStart.h"
#include "APlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class AUGHRIM_API AAPlayerStart : public APlayerStart
{
	GENERATED_BODY()

	AAPlayerStart(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = "PlayerStart")
	bool bPlayerOnly;

public:

	bool GetIsPlayerOnly() { return bPlayerOnly; }
	
};
