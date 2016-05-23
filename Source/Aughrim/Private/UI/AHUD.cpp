// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "AHUD.h"
#include "ACharacter.h"

AAHUD::AAHUD(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AAHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawCenterDot();
}

void AAHUD::DrawCenterDot()
{
	if (CenterDotIcon.Texture)
	{
		float CenterX = Canvas->ClipX / 2;
		float CenterY = Canvas->ClipY / 2;
		float CenterDotScale = 0.07f;
		
		AACharacter* Pawn = Cast<AACharacter>(GetOwningPawn());
		if (Pawn)
		{
			// Boost size when hovering over a usable object

			Canvas->SetDrawColor(255, 255, 255, 255);
			Canvas->DrawIcon(CenterDotIcon,
				CenterX - CenterDotIcon.UL * CenterDotScale / 2.0f,
				CenterY - CenterDotIcon.VL * CenterDotScale / 2.0f,
				CenterDotScale);
		}
	}
}

EHUDState AAHUD::GetCurrentState()
{
	return CurrentState;
}

void AAHUD::OnStateChanged_Implementation(EHUDState NewState)
{
	CurrentState = NewState;
}