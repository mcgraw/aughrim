// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "ATypes.h"
#include "ABaseCharacter.generated.h"

UCLASS(ABSTRACT)
class AUGHRIM_API AABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABaseCharacter(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundTakeHit;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* SoundDeath;

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

	/* Client/local call to update sprint state */
	virtual void SetSprinting(bool NewSprinting);

	float GetSprintingSpeedModifier() const;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintingSpeedModifier;

	// Character wants to run, checked during Tick to see if allowed
	UPROPERTY(Transient, Replicated)
	bool bWantsToRun;

	/* Server side call to update actual sprint state */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetSprinting(bool NewSprinting);
	void ServerSetSprinting_Implementation(bool NewSprinting);
	bool ServerSetSprinting_Validate(bool NewSprinting);

	// Targeting Methods

	bool bIsTargeting;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	float TargetingSpeedModifier;
	
	// ** 
	// Damage & Death
	// **
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "PlayerCondition", Replicated)
	float Health;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual bool CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const;

	virtual bool Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser);

	virtual void OnDeath(float KillingDamage, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser);

	virtual void FellOutOfWorld(const class UDamageType& DmgType) override;

	void SetRagdollPhysics();

	virtual void PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser, bool bKilled);

	void ReplicatedHit(float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser, bool bKilled);

	/* Holds hit data to replicate hits and eath to clients */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_LastTakeHitInfo)
	struct FTakeHitInfo LastTakeHitInfo;

	UFUNCTION()
	void OnRep_LastTakeHitInfo();

	bool bIsDying;
};
