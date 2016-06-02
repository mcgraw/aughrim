// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "APlayerState.generated.h"

UCLASS()
class AUGHRIM_API AAPlayerState : public APlayerState
{
	GENERATED_BODY()

	AAPlayerState(const class FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(Transient, Replicated)
	int32 NumPlayerKills;

	UPROPERTY(Transient, Replicated)
	int32 NumBotKills;

	UPROPERTY(Transient, Replicated)
	int32 NumDeaths;
	
	virtual void Reset() override;

public:

	void AddPlayerKill();

	void AddBotKill();

	void AddDeath();

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetPlayerKills() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetBotKills() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetDeaths() const;

};
