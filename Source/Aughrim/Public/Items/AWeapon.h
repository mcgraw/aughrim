// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ACharacter.h"
#include "AWeapon.generated.h"

UENUM()
enum class EWeaponState
{
	Idle,
	Firing,
	Equipping,
	Reloading
};

UCLASS(ABSTRACT, Blueprintable)
class AUGHRIM_API AAWeapon : public AActor
{
	GENERATED_BODY()

	virtual void PostInitializeComponents() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	float GetEquipStartedTime() const;

	float GetEquipDuration() const;

	/* last time when this weapon was switched to */
	float EquipStartedTime;

	/* how much time weapon needs to be equipped */
	float EquipDuration;

	bool bIsEquipped;

	bool bPendingEquip;

	FTimerHandle TimerHandle_HandleFiring;

	FTimerHandle EquipFinishedTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float ShotsPerMinute;

protected:

	AAWeapon(const class FObjectInitializer& ObjectInitializer);

	/* Character socket to store this item at */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	EInventorySlot StorageSlot;

	/* Pawn owner */
	class AACharacter* MyPawn;

	/* Weapon mesh; 3rd person view */
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh;

	/* Detach weapon mesh from pawn */
	void DetachMeshFromPawn();

	bool IsEquipped() const;

	bool IsAttachedToPawn() const;
	
public:	
	
	/* Get weapon mesh (needs pawn owner to determine variant) */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	USkeletalMeshComponent* GetWeaponMesh() const;

	virtual void OnUnEquip();

	void OnEquip(bool bPlayAnimation);

	/* Set the weapon's owning pawn */
	void SetOwningPawn(AACharacter* NewOwner);

	/* Get pawn owner */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	class AACharacter* GetPawnOwner();

	virtual void OnEnterInventory(AACharacter* NewOwner);

	virtual void OnLeaveInventory();

	FORCEINLINE EInventorySlot GetStorageSlot()
	{
		return StorageSlot;
	}

	/* The class to spawn in the level when dropped */
	// TODO

public:

	void StartFire();

	void StopFire();

	EWeaponState GetCurrentState() const;

	/* You can assign default values to function params, these are then optional */
	void AttachMeshToPawn(EInventorySlot Slot = EInventorySlot::Hands);

protected:

	bool CanFire();

	FVector GetAdjustedAim() const;

	FVector GetCameraDamageStartingLocation(const FVector& AimDir) const;

	FHitResult WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const;

	/* With PURE_VIRTUAL we skip implementing the function in AWeapon.cpp and can do this in subclasses instead */
	virtual void FireWeapon() PURE_VIRTUAL(AAWeapon::FireWeapon, );

private:

	void SetWeaponState(EWeaponState NewState);

	void DetermineWeaponState();

	virtual void HandleFiring();

	// Validations

	void OnBurstStarted();

	void OnBurstFinished();

	bool bWantsToFire;

	EWeaponState CurrentState;

	bool bRefiring;

	float LastFireTime;

	/* Time between shots for repeating fire */
	float TimeBetweenShots;

	/************************************************************************/
	/* Simulation & FX                                                      */
	/************************************************************************/

private:

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* EquipSound;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* MuzzleFX;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* EquipAnim;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* FireAnim;

	UParticleSystemComponent* MuzzlePSC;

	UPROPERTY(EditDefaultsOnly)
	FName MuzzleAttachPoint;

	bool bPlayingFireAnim;

	int32 BurstCounter;

protected:
	
	virtual void SimulateWeaponFire();

	virtual void StopSimulatingWeaponFire();

	FVector GetMuzzleLocation() const;

	FVector GetMuzzleDirection() const;

	UAudioComponent* PlayWeaponSound(USoundCue* SoundToPlay);

	float PlayWeaponAnimation(UAnimMontage* Animation, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

	void StopWeaponAnimation(UAnimMontage* Animation);

	/************************************************************************/
	/* Ammo & Reloading                                                     */
	/************************************************************************/

private:

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* OutOfAmmoSound;

	FTimerHandle TimerHandle_ReloadWeapon;

	FTimerHandle TimerHandle_StopReload;

protected:

	/* Time to assign on reload when no animation is found */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	float NoAnimReloadDuration;

	/* Time to assign on equip when no animation is found */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	float NoEquipAnimDuration;

	bool bPendingReload;

	void UseAmmo();

	int32 CurrentAmmo;

	int32 CurrentAmmoInClip;

	/* Weapon ammo on spawn */
	UPROPERTY(EditDefaultsOnly)
	int32 StartAmmo;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxAmmoPerClip;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* ReloadSound;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* ReloadAnim;

	virtual void ReloadWeapon();

	/* Is weapon and character currently capable of starting a reload */
	bool CanReload();

public:

	virtual void StartReload(bool bFromReplication = false);

	virtual void StopSimulateReload();

	/* Give ammo to weapon and return the amount that was not 'consumed' beyond the max count */
	int32 GiveAmmo(int32 AddAmount);

	/* Set a new total amount of ammo of weapon */
	void SetAmmoCount(int32 NewTotalAmount);

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	int32 GetCurrentAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	int32 GetCurrentAmmoInClip() const;

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	int32 GetMaxAmmoPerClip() const;

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	int32 GetMaxAmmo() const;


};
