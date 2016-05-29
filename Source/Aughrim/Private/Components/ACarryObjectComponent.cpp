// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "ACarryObjectComponent.h"


UACarryObjectComponent::UACarryObjectComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	RotateSpeed = 10.0f;

	bUsePawnControlRotation = true;
	bDoCollisionTest = false;

	// TODO: Set is replicated
}


AActor* UACarryObjectComponent::GetActorInView()
{
	// TODO: Hit test for object pickup
	return nullptr;
}

AActor* UACarryObjectComponent::GetCarriedActor()
{
	USceneComponent* ChildComp = GetChildComponent(0);
	if (ChildComp)
	{
		return ChildComp->GetOwner();
	}
	return nullptr;
}

UStaticMeshComponent* UACarryObjectComponent::GetCarriedMeshComponent()
{
	USceneComponent* ChildComp = GetChildComponent(0);
	if (ChildComp)
	{
		AActor* OwningActor = ChildComp->GetOwner();
		if (OwningActor)
		{
			return Cast<UStaticMeshComponent>(OwningActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		}
	}
	return nullptr;
}

void UACarryObjectComponent::TickComponent(float DeltaSeconds, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaSeconds, TickType, ThisTickFunction);
}

void UACarryObjectComponent::RotateActorAroundPoint(AActor* RotateActor, FVector RotationPoint, FRotator AddRotation)
{
	FVector Loc = RotateActor->GetActorLocation() - RotationPoint;
	FVector RotatedLoc = AddRotation.RotateVector(Loc);

	FVector NewLoc = RotationPoint + RotatedLoc;

	/* Compose rotators, use Quats to avoid gimbal lock */
	FQuat AQuat = FQuat(RotateActor->GetActorRotation());
	FQuat BQuat = FQuat(AddRotation);

	FRotator NewRot = FRotator(BQuat * AQuat);

	RotateActor->SetActorLocationAndRotation(NewLoc, NewRot);
}

bool UACarryObjectComponent::GetIsCarryingActor()
{
	return GetChildComponent(0) != nullptr;
}

void UACarryObjectComponent::Rotate(float DirectionYaw, float DirectionRoll)
{
	AActor* CarriedActor = GetCarriedActor();
	if (CarriedActor)
	{
		/* Retrieve the object center */
		FVector RootOrigin = GetCarriedActor()->GetRootComponent()->Bounds.Origin;
		FRotator DeltaRot = FRotator(0, DirectionYaw * RotateSpeed, DirectionRoll * RotateSpeed);

		RotateActorAroundPoint(CarriedActor, RootOrigin, DeltaRot);
	}
}