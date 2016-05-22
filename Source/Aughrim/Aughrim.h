// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#ifndef __AUGHRIM_H__
#define __AUGHRIM_H__

/// https://wiki.unrealengine.com/How_To_Use_Sessions_In_C%2B%2B

#include "Engine.h"

// NOT included by default in an empty project. Required for replication and setting of GetLifetimeReplicatedProps
#include "UnrealNetwork.h"
#include "Online.h"

// DefaultsEngine.ini should match with this list
#define COLLISION_WEAPON			ECC_GameTraceChannel1


#endif
