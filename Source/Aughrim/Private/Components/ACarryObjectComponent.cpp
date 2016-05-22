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