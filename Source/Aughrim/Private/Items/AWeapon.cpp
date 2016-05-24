// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "ACharacter.h"
#include "AWeapon.h"


AAWeapon::AAWeapon(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AAWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AAWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

}

float AAWeapon::GetEquipStartedTime() const
{
	return 0.f;
}

float AAWeapon::GetEquipDuration() const
{
	return 0.f;
}

void AAWeapon::DetachMeshFromPawn()
{

}

bool AAWeapon::IsEquipped() const
{
	return false;
}

bool AAWeapon::IsAttachedToPawn() const
{
	return false;
}

USkeletalMeshComponent* AAWeapon::GetWeaponMesh() const
{
	return Mesh;
}

void AAWeapon::OnUnEquip()
{

}

void AAWeapon::OnEquip(bool bPlayAnimation)
{

}

void AAWeapon::SetOwningPawn(AACharacter* NewOwner)
{

}

class AACharacter* AAWeapon::GetPawnOwner()
{
	return MyPawn;
}

void AAWeapon::OnEnterInventory(AACharacter* NewOwner)
{

}

void AAWeapon::OnLeaveInventory()
{

}

void AAWeapon::StartFire()
{

}

void AAWeapon::StopFire()
{

}

EWeaponState AAWeapon::GetCurrentState() const
{
	return CurrentState;
}

void AAWeapon::AttachMeshToPawn(EInventorySlot Slot /*= EInventorySlot::Hands*/)
{

}

bool AAWeapon::CanFire()
{
	return false;
}

FVector AAWeapon::GetAdjustedAim() const
{
	return FVector::ZeroVector;
}

FVector AAWeapon::GetCameraDamageStartingLocation(const FVector& AimDir) const
{
	return FVector::ZeroVector;
}

FHitResult AAWeapon::WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const
{
	FHitResult Hit(ForceInit);
	return Hit;
}

void AAWeapon::SetWeaponState(EWeaponState NewState)
{

}

void AAWeapon::DetermineWeaponState()
{

}

void AAWeapon::HandleFiring()
{

}

void AAWeapon::OnBurstStarted()
{

}

void AAWeapon::OnBurstFinished()
{

}

void AAWeapon::SimulateWeaponFire()
{

}

void AAWeapon::StopSimulatingWeaponFire()
{

}

FVector AAWeapon::GetMuzzleLocation() const
{
	return FVector::ZeroVector;
}

FVector AAWeapon::GetMuzzleDirection() const
{
	return FVector::ZeroVector;
}

UAudioComponent* AAWeapon::PlayWeaponSound(USoundCue* SoundToPlay)
{
	UAudioComponent* AC = nullptr;
	if (SoundToPlay && MyPawn)
	{
		AC = UGameplayStatics::SpawnSoundAttached(SoundToPlay, MyPawn->GetRootComponent());
	}
	return AC;
}

float AAWeapon::PlayWeaponAnimation(UAnimMontage* Animation, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	return 0.f;
}

void AAWeapon::StopWeaponAnimation(UAnimMontage* Animation)
{

}

void AAWeapon::UseAmmo()
{

}

void AAWeapon::ReloadWeapon()
{

}

bool AAWeapon::CanReload()
{
	return false;
}

void AAWeapon::StartReload(bool bFromReplication /*= false*/)
{

}

void AAWeapon::StopSimulateReload()
{

}

int32 AAWeapon::GiveAmmo(int32 AddAmount)
{
	return 0;
}

void AAWeapon::SetAmmoCount(int32 NewTotalAmount)
{

}

int32 AAWeapon::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

int32 AAWeapon::GetCurrentAmmoInClip() const
{
	return CurrentAmmoInClip;
}

int32 AAWeapon::GetMaxAmmoPerClip() const
{
	return MaxAmmoPerClip;
}

int32 AAWeapon::GetMaxAmmo() const
{
	return MaxAmmo;
}
