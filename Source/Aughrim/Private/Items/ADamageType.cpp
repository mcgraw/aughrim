// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "ADamageType.h"


UADamageType::UADamageType(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HeadDmgModifier = 2.0f;
	LimbDmgModifier = 0.5f;

	bCanDieFrom = true;
}

bool UADamageType::GetCanDieFrom()
{
	return bCanDieFrom;
}

float UADamageType::GetHeadDamageModifier()
{
	return HeadDmgModifier;
}

float UADamageType::GetLimbDamageModifier()
{
	return LimbDmgModifier;
}
