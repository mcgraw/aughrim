// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "AughrimHUD.generated.h"

UCLASS()
class AAughrimHUD : public AHUD
{
	GENERATED_BODY()

public:
	AAughrimHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Display")
	bool hideHUD;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

