// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "ACharacter.h"
#include "AWeapon.h"
#include "AWeaponPickup.h"
#include "APlayerController.h"


AAWeaponPickup::AAWeaponPickup(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAllowRespawn = false;

	/* Enabled to support simulated physics movement when weapons are dropped by a player */
	bReplicateMovement = true;
}

void AAWeaponPickup::OnUsed(APawn* InstigatorPawn)
{
	AACharacter* MyPawn = Cast<AACharacter>(InstigatorPawn);
	if (MyPawn)
	{
		/* Fetch the default variables of the class we are about to pick up and check if the storage slot is available on the pawn */
		if (MyPawn->WeaponSlotAvailable(WeaponClass->GetDefaultObject<AAWeapon>()->GetStorageSlot()))
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AAWeapon* NewWeapon = GetWorld()->SpawnActor<AAWeapon>(WeaponClass, SpawnInfo);

			MyPawn->AddWeapon(NewWeapon);

			Super::OnUsed(InstigatorPawn);
		}
		else
		{
			AAPlayerController* PC = Cast<AAPlayerController>(MyPawn->GetController());
			if (PC)
			{
				// HUD message; slot taken
			}

		}

	}
}
