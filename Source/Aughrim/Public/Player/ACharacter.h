// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ABaseCharacter.h"
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

	/* Is character currently performing a jump action. Resets on landed. */
	UPROPERTY(Transient, Replicated)
	bool bIsJumping;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsInitiatedJump() const;

	void SetIsJumping(bool NewJumping);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSetIsJumping(bool NewJumping);
	void ServerSetIsJumping_Implementation(bool NewJumping);
	bool ServerSetIsJumping_Validate(bool NewJumping);


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

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AAWeapon* GetCurrentWeapon() const;

	bool WeaponSlotAvailable(EInventorySlot CheckSlot);

	bool CanFire() const;
	bool CanReload() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool IsFiring() const;

	FName GetInventoryAttachPoint(EInventorySlot Slot) const;

	/* All weapons/items the player currently holds */
	UPROPERTY(Transient, Replicated)
	TArray<AAWeapon*> Inventory;

	void SetCurrentWeapon(class AAWeapon* NewWeapon, class AAWeapon* LastWeapon = nullptr);

	void EquipWeapon(AAWeapon* Weapon);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerEquipWeapon(AAWeapon* Weapon);
	void ServerEquipWeapon_Implementation(AAWeapon* Weapon);
	bool ServerEquipWeapon_Validate(AAWeapon* Weapon);
	
	/* OnRep func can use a param to hold the previous value of the variable. Very useful when you need to handle UnEquip */
	UFUNCTION()
	void OnRep_CurrentWeapon(AAWeapon* LastWeapon);

	void AddWeapon(class AAWeapon* Weapon);

	void RemoveWeapon(class AAWeapon* Weapon, bool bDestroy);

	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentWeapon)
	class AAWeapon* CurrentWeapon;

	class AAWeapon* PreviousWeapon;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SwapToNewWeaponMesh();
};
