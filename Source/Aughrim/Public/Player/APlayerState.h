// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "APlayerState.generated.h"

UCLASS()
class AUGHRIM_API AAPlayerState : public APlayerState
{
	GENERATED_BODY()

	AAPlayerState(const class FObjectInitializer& ObjectInitializer);
	
	int32 NumKills;

	int32 NumDeaths;

	virtual void Reset() override;

public:

	void AddKill();

	void AddDeath();

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetKills() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetDeaths() const;

};
