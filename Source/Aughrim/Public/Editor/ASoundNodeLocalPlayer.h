// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sound/SoundNode.h"
#include "ASoundNodeLocalPlayer.generated.h"

UCLASS(hidecategories = Object, editinlinenew)
class AUGHRIM_API UASoundNodeLocalPlayer : public USoundNode
{
	GENERATED_BODY()

		virtual void ParseNodes(FAudioDevice* AudioDevice, const UPTRINT NodeWaveInstanceMesh, FActiveSound& ActiveSound, const FSoundParseParameters& ParseParams, TArray<FWaveInstance*>& WaveInstances) override;

		virtual void CreateStartingConnectors(void) override;

		virtual int32 GetMaxChildNodes() const override;

		virtual int32 GetMinChildNodes() const override;

#if WITH_EDITOR

		virtual FText GetInputPinName(int32 PinIndex) const override;

#endif
	
};
