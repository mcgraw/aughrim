// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Player/ABaseCharacter.h"
#include "ACharacter.generated.h"

UCLASS()
class AUGHRIM_API AACharacter : public AABaseCharacter
{
	GENERATED_BODY()

	AACharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void PawnClientRestart() override;

private:

	// Boom to handle distance to player mesh
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* CameraBoomComp;

	// Primary camera of the player
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UACarryObjectComponent* CarriedObjectComp;
	
public:

	FORCEINLINE UCameraComponent* GetCameraComponent()
	{
		return CameraComp;
	}
	
	// **
	// Movement
	// **

	virtual void MoveForward(float Val);

	virtual void MoveRight(float Val);

	// Client mapped to Input
	void OnStartJump();
	void OnStopJump();

	void OnStartSprinting();
	void OnStopSprinting();

	virtual void SetSprinting(bool NewSprinting) override;

	bool bIsJumping;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsInitiatedJump() const;

	void SetIsJumping(bool NewJumping);

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	// **
	// Object Interaction
	// **

	// Input mapped function for carry object component
	void OnToggleCarryActor();


	// **
	// Targeting
	// **


	// **
	// Damage & Death
	// **


private:

	// Attach point for active weapon/item in hand
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
	FName WeaponAttachPoint;

	bool bWantsToFire;

	void OnReload();

	// Mapped to Input
	void OnStartFire();
	void OnStopFire();

	void StartWeaponFire();
	void StopWeaponFire();

public:

	//UFUNCTION(BlueprintCallable, Category = "Weapon")
	//AAWeapon* GetCurrentWeapon() const;

};
