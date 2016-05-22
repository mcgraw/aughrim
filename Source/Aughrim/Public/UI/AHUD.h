// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "AHUD.generated.h"

UENUM(BlueprintType)
enum class EHUDState : uint8
{
	Playing,
	Spectating,
	MatchEnd
};

UCLASS()
class AUGHRIM_API AAHUD : public AHUD
{
	GENERATED_BODY()

	AAHUD(const class FObjectInitializer& ObjectInitializer);

	/* Current HUD State */
	EHUDState CurrentState;

	/* Update loop */
	virtual void DrawHUD() override;

	void DrawCenterDot();

public:

	UPROPERTY(BlueprintReadWrite, Category = "HUD")
	FCanvasIcon CenterDotIcon;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	EHUDState GetCurrentState();
	
	/* Event hook to call HUD text events to display. Blueprint HUD class must implement how to deal with this event */
	UFUNCTION(BlueprintImplementableEvent, Category = "HUDEvents")
	void MessageReceived(const FText& TextMessage);

	/* Event hook to update HUD state (eg. to determine visibility of widgets) */
	UFUNCTION(BlueprintNativeEvent, Category = "HUDEvents")
	void OnStateChanged(EHUDState NewState);
};
