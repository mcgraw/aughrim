// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "APlayerController.h"
#include "ACharacter.h"
#include "AWeapon.h"


AAWeapon::AAWeapon(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Mesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh3P"));
	Mesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	Mesh->bReceivesDecals = true;
	Mesh->CastShadow = true;
	Mesh->SetCollisionObjectType(ECC_WorldDynamic);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	RootComponent = Mesh;

	bIsEquipped = true;
	CurrentState = EWeaponState::Idle;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	SetReplicates(true);
	bNetUseOwnerRelevancy = true;

	MuzzleAttachPoint = TEXT("MuzzleFlashSocket");
	StorageSlot = EInventorySlot::Primary;

	ShotsPerMinute = 700;
	StartAmmo = 999;
	MaxAmmo = 999;
	MaxAmmoPerClip = 30;
	NoAnimReloadDuration = 1.5f;
	NoEquipAnimDuration = 0.5f;
}

void AAWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TimeBetweenShots = 60.0f / ShotsPerMinute;
	CurrentAmmo = FMath::Min(StartAmmo, MaxAmmo);
	CurrentAmmoInClip = FMath::Min(MaxAmmoPerClip, StartAmmo);
}

void AAWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	DetachMeshFromPawn();
	StopSimulatingWeaponFire();
}

float AAWeapon::GetEquipStartedTime() const
{
	return EquipStartedTime;
}

float AAWeapon::GetEquipDuration() const
{
	return EquipDuration;
}

void AAWeapon::OnRep_MyPawn()
{

}

void AAWeapon::DetachMeshFromPawn()
{
	Mesh->DetachFromParent();
	Mesh->SetHiddenInGame(true);
}

void AAWeapon::OnEquipFinished()
{
	AttachMeshToPawn();

	bIsEquipped = true;
	bPendingEquip = false;

	DetermineWeaponState();

	if (MyPawn)
	{
		if (MyPawn->IsLocallyControlled() && CurrentAmmoInClip <= 0 && CanReload())
		{
			StartReload();
		}
	}
}

bool AAWeapon::IsEquipped() const
{
	return bIsEquipped;
}

bool AAWeapon::IsAttachedToPawn() const
{
	return bIsEquipped || bPendingEquip;
}

USkeletalMeshComponent* AAWeapon::GetWeaponMesh() const
{
	return Mesh;
}

void AAWeapon::OnUnEquip()
{
	bIsEquipped = false;
	StopFire();

	if (bPendingEquip)
	{
		StopWeaponAnimation(EquipAnim);
		bPendingEquip = false;

		GetWorldTimerManager().ClearTimer(EquipFinishedTimerHandle);
	}
	if (bPendingReload)
	{
		StopWeaponAnimation(ReloadAnim);
		bPendingReload = false;

		GetWorldTimerManager().ClearTimer(TimerHandle_ReloadWeapon);
	}

	DetermineWeaponState();
}

void AAWeapon::OnEquip(bool bPlayAnimation)
{
	bPendingEquip = true;
	DetermineWeaponState();

	if (bPlayAnimation)
	{
		float Duration = PlayWeaponAnimation(EquipAnim);
		if (Duration <= 0.0f)
		{
			Duration = NoEquipAnimDuration;
		}
		EquipStartedTime = GetWorld()->TimeSeconds;
		EquipDuration = Duration;

		GetWorldTimerManager().SetTimer(EquipFinishedTimerHandle, this, &AAWeapon::OnEquipFinished, Duration, false);
	} 
	else
	{
		OnEquipFinished();
	}

	if (MyPawn && MyPawn->IsLocallyControlled())
	{
		PlayWeaponSound(EquipSound);
	}
}

void AAWeapon::SetOwningPawn(AACharacter* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;

		// Net owner for RPC calls
		SetOwner(NewOwner);
	}
}

class AACharacter* AAWeapon::GetPawnOwner()
{
	return MyPawn;
}

void AAWeapon::OnEnterInventory(AACharacter* NewOwner)
{
	SetOwningPawn(NewOwner);
	AttachMeshToPawn(StorageSlot);
}

void AAWeapon::OnLeaveInventory()
{
	// NET

	if (IsAttachedToPawn())
	{
		OnUnEquip();
	}

	DetachMeshFromPawn();
}

void AAWeapon::StartFire()
{
	if (Role < ROLE_Authority)
	{
		ServerStartFire();
	}
	
	if (!bWantsToFire)
	{
		bWantsToFire = true;
		DetermineWeaponState();
	}
}

void AAWeapon::StopFire()
{
	if (Role < ROLE_Authority)
	{
		ServerStopFire();
	}

	if (bWantsToFire)
	{
		bWantsToFire = false;
		DetermineWeaponState();
	}
}

EWeaponState AAWeapon::GetCurrentState() const
{
	return CurrentState;
}

void AAWeapon::AttachMeshToPawn(EInventorySlot Slot /*= EInventorySlot::Hands*/)
{
	if (MyPawn)
	{
		DetachMeshFromPawn();

		USkeletalMeshComponent* PawnMesh = MyPawn->GetMesh();
		FName AttachPoint = MyPawn->GetInventoryAttachPoint(Slot);
		Mesh->SetHiddenInGame(false);
		Mesh->AttachTo(PawnMesh, AttachPoint, EAttachLocation::SnapToTarget);
	}
}

bool AAWeapon::CanFire()
{
	bool bPawnCanFire = MyPawn && MyPawn->CanFire();
	bool bStateOK = CurrentState == EWeaponState::Idle || CurrentState == EWeaponState::Firing;
	return bPawnCanFire && bStateOK && !bPendingReload;
}

FVector AAWeapon::GetAdjustedAim() const
{
	AAPlayerController* const PC = Instigator ? Cast<AAPlayerController>(Instigator->Controller) : nullptr;
	FVector FinalAim = FVector::ZeroVector;

	if (PC)
	{
		FVector CamLoc;
		FRotator CamRot;
		PC->GetPlayerViewPoint(CamLoc, CamRot);

		FinalAim = CamRot.Vector();
	}
	else if (Instigator)
	{
		FinalAim = Instigator->GetBaseAimRotation().Vector();
	}
	return FinalAim;
}

FVector AAWeapon::GetCameraDamageStartLocation(const FVector& AimDir) const
{
	AAPlayerController* PC = MyPawn ? Cast<AAPlayerController>(MyPawn->Controller) : nullptr;
	FVector OutStartTrace = FVector::ZeroVector;

	if (PC)
	{
		FRotator DummyRot;
		PC->GetPlayerViewPoint(OutStartTrace, DummyRot);

		// Adjust trace so there is nothing blocking the ray between the camera and the pawn, and calc distance from adjusted start
		OutStartTrace = OutStartTrace + AimDir * (FVector::DotProduct((Instigator->GetActorLocation() - OutStartTrace), AimDir));
	}

	return OutStartTrace;
}

FHitResult AAWeapon::WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const
{
	FCollisionQueryParams TraceParams(TEXT("WeaponTrace"), true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, COLLISION_WEAPON, TraceParams);

	return Hit;
}

void AAWeapon::SetWeaponState(EWeaponState NewState)
{
	const EWeaponState PrevState = CurrentState;

	if (PrevState == EWeaponState::Firing && NewState != EWeaponState::Firing)
	{
		OnBurstFinished();
	}

	CurrentState = NewState;

	if (PrevState != EWeaponState::Firing && NewState == EWeaponState::Firing)
	{
		OnBurstStarted();
	}
}

void AAWeapon::DetermineWeaponState()
{
	EWeaponState NewState = EWeaponState::Idle;

	if (bIsEquipped)
	{
		if (bPendingReload)
		{
			if (CanReload())
			{
				NewState = EWeaponState::Reloading;
			}
			else
			{
				NewState = CurrentState;
			}
		}
		else if (!bPendingReload && bWantsToFire && CanFire())
		{
			NewState = EWeaponState::Firing;
		}
	}
	else if (bPendingEquip)
	{
		NewState = EWeaponState::Equipping;
	}

	SetWeaponState(NewState);
}

void AAWeapon::HandleFiring()
{
	if (CurrentAmmoInClip > 0 && CanFire())
	{
		if (GetNetMode() != NM_DedicatedServer)
		{
			SimulateWeaponFire();
		}

		if (MyPawn && MyPawn->IsLocallyControlled())
		{
			FireWeapon();

			UseAmmo();

			BurstCounter++;
		}
	}
	else if (CanReload())
	{
		StartReload();
	}
	else if (MyPawn && MyPawn->IsLocallyControlled())
	{
		if (GetCurrentAmmo() == 0 && !bRefiring)
		{
			PlayWeaponSound(OutOfAmmoSound);
		}

		/* Reload after firing last round */
		if (CurrentAmmoInClip <= 0 && CanReload())
		{
			StartReload();
		}

		/* Stop weapon fire FX, but stay in firing state */
		if (BurstCounter > 0)
		{
			OnBurstFinished();
		}
	}

	if (MyPawn && MyPawn->IsLocallyControlled())
	{
		if (Role < ROLE_Authority)
		{
			ServerHandleFiring();
		}

		/* Retrigger HandleFiring on a delay for automatic weapons */
		bRefiring = (CurrentState == EWeaponState::Firing && TimeBetweenShots > 0.0f);
		if (bRefiring)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &AAWeapon::HandleFiring, TimeBetweenShots, false);
		}
	}

	LastFireTime = GetWorld()->GetTimeSeconds();
}

void AAWeapon::ServerStartFire_Implementation()
{
	StartFire();
}

bool AAWeapon::ServerStartFire_Validate()
{
	return true;
}

void AAWeapon::ServerStopFire_Implementation()
{
	StopFire();
}

bool AAWeapon::ServerStopFire_Validate()
{
	return true;
}

void AAWeapon::ServerHandleFiring_Implementation()
{
	const bool bShouldUpdateAmmo = (CurrentAmmoInClip > 0 && CanFire());

	HandleFiring();

	if (bShouldUpdateAmmo)
	{
		UseAmmo();

		BurstCounter++;
	}
}

bool AAWeapon::ServerHandleFiring_Validate()
{
	return true;
}

void AAWeapon::OnBurstStarted()
{
	// Start firing, can be delayed to satisfy TimeBetweenShots
	const float GameTime = GetWorld()->GetTimeSeconds();
	if (LastFireTime > 0 && TimeBetweenShots > 0.0f && LastFireTime + TimeBetweenShots > GameTime)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &AAWeapon::HandleFiring, LastFireTime + TimeBetweenShots - GameTime, false);
	}
	else
	{
		HandleFiring();
	}
}

void AAWeapon::OnBurstFinished()
{
	BurstCounter = 0;

	StopSimulatingWeaponFire();

	GetWorldTimerManager().ClearTimer(TimerHandle_HandleFiring);
	bRefiring = false;
}

void AAWeapon::OnRep_BurstCounter()
{
	if (BurstCounter > 0)
	{
		SimulateWeaponFire();
	}
	else
	{
		StopSimulatingWeaponFire();
	}
}

void AAWeapon::SimulateWeaponFire()
{
	if (MuzzleFX)
	{
		MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, Mesh, MuzzleAttachPoint);
	}

	if (!bPlayingFireAnim)
	{
		PlayWeaponAnimation(FireAnim);
		bPlayingFireAnim = true;
	}

	PlayWeaponSound(FireSound);
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
	UE_LOG(LogTemp, Warning, TEXT("PLAY SOUND"));
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

void AAWeapon::OnRep_Reload()
{
	if (bPendingReload)
	{
		/* By passing true we do not push back to server and execute it locally */
		StartReload(true);
	}
	else
	{
		StopSimulateReload();
	}
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

void AAWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAWeapon, MyPawn);

	DOREPLIFETIME_CONDITION(AAWeapon, CurrentAmmo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AAWeapon, CurrentAmmoInClip, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AAWeapon, BurstCounter, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AAWeapon, bPendingReload, COND_SkipOwner);
}