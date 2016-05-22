// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "ABaseCharacter.generated.h"

UCLASS(ABSTRACT)
class AUGHRIM_API AABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABaseCharacter(const class FObjectInitializer& ObjectInitializer);

	// **
	// Health
	// **

	UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerCondition")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual bool IsSprinting() const;

	// Client/local call to update sprint state
	virtual void SetSprinting(bool NewSprinting);

	float GetSprintingSpeedModifier() const;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintingSpeedModifier;

	// Character wants to run, checked during Tick to see if allowed
	bool bWantsToRun;

	// Sprinting Methods

	// Targeting Methods

	bool bIsTargeting;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	float TargetingSpeedModifier;
	
	// ** 
	// Damage & Death
	// **
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "PlayerCondition")
	float Health;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	// Can Die

	// Die

	// On Death

	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;
};
