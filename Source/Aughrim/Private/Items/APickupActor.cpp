// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "APickupActor.h"

AAPickupActor::AAPickupActor(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	/* Ignore Pawn - this is to prevent objects shooting throuhg the level or pawns glitching on top of small items. */
	MeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	bIsActive = false;
	bStartActive = true;
	bAllowRespawn = true;
	RespawnDelay = 5.0f;
	RespawnDelayRange = 5.0f;

	// NET
}

void AAPickupActor::RespawnPickup()
{
	bIsActive = true;
	OnRespawned();
}

void AAPickupActor::OnPickedUp()
{
	if (MeshComp)
	{
		MeshComp->SetVisibility(false);
		MeshComp->SetSimulatePhysics(false);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AAPickupActor::OnRespawned()
{
	if (MeshComp)
	{
		MeshComp->SetVisibility(true);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void AAPickupActor::OnUsed(APawn* InstigatorPawn)
{
	Super::OnUsed(InstigatorPawn);

	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());

	bIsActive = false;
	OnPickedUp();

	if (bAllowRespawn)
	{
		FTimerHandle RespawnTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AAPickupActor::RespawnPickup, RespawnDelay + FMath::RandHelper(RespawnDelayRange), false);
	}
	else
	{
		/* Delete from level if respawn is not allowed */
		Destroy();
	}
}

void AAPickupActor::BeginPlay()
{
	Super::BeginPlay();

	//if (bStartActive)
	{
		RespawnPickup();
	}
}

