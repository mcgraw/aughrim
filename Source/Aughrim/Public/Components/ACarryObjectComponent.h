// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "ACarryObjectComponent.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class AUGHRIM_API UACarryObjectComponent : public USpringArmComponent
{
	GENERATED_BODY()

	UACarryObjectComponent(const FObjectInitializer& ObjectInitializer);

private:

	AActor* GetActorInView();

	AActor* GetCarriedActor();
	
	// Return the StaticMeshComponent of the carried Actor
	UStaticMeshComponent* GetCarriedMeshComponent();

protected:

	void TickComponent(float DeltaSeconds, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	// Rotation speed
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float RotateSpeed;

	void RotateActorAroundPoint(AActor* RotateActor, FVector RotationPoint, FRotator AddRotation);

	/// Explore picking up and dropping items

public:

	/* Is currently holding an Actor */
	bool GetIsCarryingActor();

	/* Rotate the carried Actor */
	void Rotate(float DirectionYaw, float DirectionRoll);

	// NET
};
