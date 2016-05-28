// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "AUsableActor.h"

AAUsableActor::AAUsableActor(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = MeshComp;
}

void AAUsableActor::OnUsed(APawn* InstigatorPawn)
{
	// Nothing to do here...
}

void AAUsableActor::OnBeginFocus()
{
	// Used by custom PostProcess to render outlines
	MeshComp->SetRenderCustomDepth(true);
}

void AAUsableActor::OnEndFocus()
{
	// Used by custom PostProcess to render outlines
	MeshComp->SetRenderCustomDepth(false);
}

