// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DamageType.h"
#include "ADamageType.generated.h"

UCLASS()
class AUGHRIM_API UADamageType : public UDamageType
{
	GENERATED_BODY()

	UADamageType(const class FObjectInitializer& ObjectInitializer);

	/* Can player die from this damage type */
	UPROPERTY(EditDefaultsOnly)
	bool bCanDieFrom;

	/* Damage modifier for headshot damage */
	UPROPERTY(EditDefaultsOnly)
	float HeadDmgModifier;

	UPROPERTY(EditDefaultsOnly)
	float LimbDmgModifier;

public:

	bool GetCanDieFrom();

	float GetHeadDamageModifier();

	float GetLimbDamageModifier();	
	
};
