// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "ACharacter.h"
#include "AWeapon.h"
#include "ACarryObjectComponent.h"

AACharacter::AACharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	// Adjust jump to make it less floaty
	MoveComp->GravityScale = 1.5f;
	MoveComp->JumpZVelocity = 620;
	MoveComp->bCanWalkOffLedgesWhenCrouching = true;
	MoveComp->MaxWalkSpeedCrouched = 200;

	// Ignore this channel or it will absorb the trace impacts instead of the skeletal mesh
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	// Enable crouching
	MoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;

	CameraBoomComp = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoomComp->SocketOffset = FVector(0, 35, 0);
	CameraBoomComp->TargetOffset = FVector(0, 0, 55);
	CameraBoomComp->bUsePawnControlRotation = true;
	CameraBoomComp->AttachParent = GetRootComponent();

	CameraComp = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Camera"));
	CameraComp->AttachParent = CameraBoomComp;

	CarriedObjectComp = ObjectInitializer.CreateDefaultSubobject<UACarryObjectComponent>(this, TEXT("CarriedObjectComp"));
	CarriedObjectComp->AttachParent = GetRootComponent();

	TargetingSpeedModifier = 0.5f;
	SprintingSpeedModifier = 2.5f;

	Health = 100;

	/* Names as specified in the character skeleton */
	WeaponAttachPoint = TEXT("WeaponSocket");
}

void AACharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AACharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AACharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	// Movement
	InputComponent->BindAxis("MoveForward", this, &AACharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AACharacter::MoveRight);
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// Sprint
	InputComponent->BindAction("SprintHold", IE_Pressed, this, &AACharacter::OnStartSprinting);
	InputComponent->BindAction("SprintHold", IE_Released, this, &AACharacter::OnStopSprinting);

	// Crouch
	// InputComponent->BindAction("CrouchToggle", IE_Released, this, &AACharacter::OnCrouchToggle);

	// Jump
	InputComponent->BindAction("Jump", IE_Pressed, this, &AACharacter::OnStartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AACharacter::OnStopJump);

	// Weapons
	InputComponent->BindAction("Fire", IE_Pressed, this, &AACharacter::OnStartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &AACharacter::OnStopFire);

	InputComponent->BindAction("Reload", IE_Pressed, this, &AACharacter::OnReload);

}

void AACharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AACharacter::PawnClientRestart()
{
	Super::PawnClientRestart();
}

void AACharacter::MoveForward(float Val)
{
	if (Controller && Val != 0.f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Val);
	}
}

void AACharacter::MoveRight(float Val)
{
	if (Val != 0.f)
	{
		const FRotator Rotation = GetActorRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Val);
	}
}

void AACharacter::OnStartJump()
{
	bPressedJump = true;
	SetIsJumping(true);
}

void AACharacter::OnStopJump()
{
	bPressedJump = false;
}

void AACharacter::OnStartSprinting()
{
	SetSprinting(true);
}

void AACharacter::OnStopSprinting()
{
	SetSprinting(false);
}

void AACharacter::SetSprinting(bool NewSprinting)
{
	if (bWantsToRun)
	{
		StopWeaponFire();
	}
	Super::SetSprinting(NewSprinting);
}

bool AACharacter::IsInitiatedJump() const
{
	return bIsJumping;
}

void AACharacter::SetIsJumping(bool NewJumping)
{
	bIsJumping = NewJumping;
}

void AACharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode /*= 0*/)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (PrevMovementMode == EMovementMode::MOVE_Falling && GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling)
	{
		SetIsJumping(false);
	}
}

void AACharacter::OnToggleCarryActor()
{
	//CarriedObjectComp->Pickup()
}

void AACharacter::OnReload()
{

}

void AACharacter::OnStartFire()
{
	if (CarriedObjectComp->GetIsCarryingActor())
	{
		CarriedObjectComp->Rotate(1.0f, 1.0f);
		return;
	}

	StartWeaponFire();
}

void AACharacter::OnStopFire()
{
	StopWeaponFire();
}

void AACharacter::StartWeaponFire()
{
	if (!bWantsToFire)
	{
		bWantsToFire = true;
		if (CurrentWeapon)
		{
			CurrentWeapon->StartFire();
		}
	}
}

void AACharacter::StopWeaponFire()
{
	if (bWantsToFire)
	{
		bWantsToFire = false;
		if (CurrentWeapon)
		{
			CurrentWeapon->StopFire();
		}
	}
}

AAWeapon* AACharacter::GetCurrentWeapon() const
{
	return CurrentWeapon;
}

bool AACharacter::WeaponSlotAvailable(EInventorySlot CheckSlot)
{
	return false;
}

bool AACharacter::CanFire() const
{
	/* Add more checks for non-shooting areas, or NPC dialogs */
	return IsAlive();
}

bool AACharacter::CanReload() const
{
	return IsAlive();
}

bool AACharacter::IsFiring() const
{
	return CurrentWeapon && CurrentWeapon->GetCurrentState() == EWeaponState::Firing;
}

FName AACharacter::GetInventoryAttachPoint(EInventorySlot Slot) const
{
	switch (Slot)
	{
	case EInventorySlot::Hands:
		return WeaponAttachPoint;
	default:
		return ""; // Not Implemented
	}
}

void AACharacter::SetCurrentWeapon(class AAWeapon* NewWeapon, class AAWeapon* LastWeapon /*= nullptr*/)
{
	/* Maintain a reference for visual weapon swapping */
	PreviousWeapon = LastWeapon;

	AAWeapon* LocalLastWeapon = nullptr;
	if (LastWeapon)
	{
		LocalLastWeapon = LastWeapon;
	}
	else if (NewWeapon != CurrentWeapon)
	{
		LocalLastWeapon = CurrentWeapon;
	}

	// UnEquip the current
	bool bHasPreviousWeapon = false;
	if (LocalLastWeapon)
	{
		LocalLastWeapon->OnUnEquip();
		bHasPreviousWeapon = true;
	}

	CurrentWeapon = NewWeapon;

	if (NewWeapon)
	{
		NewWeapon->SetOwningPawn(this);

		NewWeapon->OnEquip(bHasPreviousWeapon);
	}

	/* No equip animation w/ animnotify? uncomment to immediately swap */
	// SwapToNewWeaponMesh();
}

void AACharacter::EquipWeapon(AAWeapon* Weapon)
{
	if (Weapon)
	{
		/* Ignore if trying to equip already equipped weapon */
		if (Weapon == CurrentWeapon)
			return;

		// ROLE_Authority check

		SetCurrentWeapon(Weapon, CurrentWeapon);
	}
}

void AACharacter::AddWeapon(class AAWeapon* Weapon)
{
	// ROLE_Authority check

	if (Weapon)
	{
		Weapon->OnEnterInventory(this);
		Inventory.AddUnique(Weapon);

		// Equip first weapon in inventory
		if (Inventory.Num() > 0 && CurrentWeapon == nullptr)
		{
			EquipWeapon(Inventory[0]);
		}
	}
}

void AACharacter::RemoveWeapon(class AAWeapon* Weapon, bool bDestroy)
{
	// ROLE_Authority check

	if (Weapon)
	{
		bool bIsCurrent = CurrentWeapon == Weapon;

		if (Inventory.Contains(Weapon))
		{
			Weapon->OnLeaveInventory();
		}
		Inventory.RemoveSingle(Weapon);

		/* Replace weapon if we removed our current weapon */
		if (bIsCurrent && Inventory.Num() > 0)
		{
			SetCurrentWeapon(Inventory[0]);
		}

		/* Clear reference to weapon if we have no items left in inventory */
		if (Inventory.Num() == 0)
		{
			SetCurrentWeapon(nullptr);
		}

		if (bDestroy)
		{
			Weapon->Destroy();
		}
	}
}

void AACharacter::SwapToNewWeaponMesh()
{
	if (PreviousWeapon)
	{
		PreviousWeapon->AttachMeshToPawn(PreviousWeapon->GetStorageSlot());
	}

	if (CurrentWeapon)
	{
		CurrentWeapon->AttachMeshToPawn(EInventorySlot::Hands);
	}
}
