// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "AGameMode.h"
#include "APlayerController.h"
#include "ACharacter.h"
#include "AWeapon.h"
#include "APlayerState.h"
#include "APlayerStart.h"
#include "AGameState.h"

AAGameMode::AAGameMode(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	/* Assign class types used by this mode */
	PlayerControllerClass = AAPlayerController::StaticClass();
	PlayerStateClass = AAPlayerState::StaticClass();
	GameStateClass = AAGameState::StaticClass();
}

void AAGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	/* Set timer to run every second */
	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AAGameMode::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);
}

void AAGameMode::InitGameState()
{
	Super::InitGameState();
}

void AAGameMode::DefaultTimer()
{

}

void AAGameMode::StartMatch()
{
	// Introduce spawning bots at interval

	Super::StartMatch();
}

void AAGameMode::SpawnDefaultInventory(APawn* PlayerPawn)
{
	AACharacter* MyPawn = Cast<AACharacter>(PlayerPawn);
	if (MyPawn)
	{
		for (int32 i = 0; i < DefaultInventoryClasses.Num(); i++)
		{
			if (DefaultInventoryClasses[i])
			{
				FActorSpawnParameters SpawnInfo;
				SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				AAWeapon* NewWeapon = GetWorld()->SpawnActor<AAWeapon>(DefaultInventoryClasses[i], SpawnInfo);

				MyPawn->AddWeapon(NewWeapon);
			}
		}
	}
}

void AAGameMode::SetPlayerDefaults(APawn* PlayerPawn)
{
	Super::SetPlayerDefaults(PlayerPawn);

	SpawnDefaultInventory(PlayerPawn);
}

FString AAGameMode::InitNewPlayer(class APlayerController* NewPlayerController, const TSharedPtr<const FUniqueNetId>& UniqueId, const FString& Options, const FString& Portal /*= TEXT("")*/)
{
	FString Result = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
	UE_LOG(LogTemp, Warning, TEXT("InitNewPlayer: %s"), Result);
	return Result;
}

bool AAGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	/* Always pick a random location */
	return false;
}

bool AAGameMode::IsSpawnpointAllowed(APlayerStart* SpawnPoint, AController* Controller)
{
	if (Controller == nullptr || Controller->PlayerState == nullptr)
		return true;

	AAPlayerStart* MyPlayerStart = Cast<AAPlayerStart>(SpawnPoint);
	if (MyPlayerStart)
	{
		return MyPlayerStart->GetIsPlayerOnly() && !Controller->PlayerState->bIsABot;
	}

	/* Cast failed, anyone can spawn at the base playerstart class */
	return true;
}

bool AAGameMode::IsSpawnpointPreferred(APlayerStart* SpawnPoint, AController* Controller)
{
	return false;
}

UClass* AAGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void AAGameMode::Killed(AController* Killer, AController* VictimPlayer, APawn* VictimPawn, const UDamageType* DamageType)
{

}

bool AAGameMode::CanDealDamage(class ASPlayerState* DamageCauser, class ASPlayerState* DamagedPlayer) const
{
	return true;
}

float AAGameMode::ModifyDamage(float Damage, AActor* DamagedActor, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) const
{
	return Damage;
}

