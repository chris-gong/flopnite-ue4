// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"

#define PRINT_TO_SCREEN(...) if(GEngine) GEngine->AddOnScreenDebugMessage(##__VA_ARGS__);

//Current Class where this is called!
#define GET_CUR_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) )

//Current Line Number in the code where this is called!
#define GET_CUR_LINE  (FString::FromInt(__LINE__))

//Current Class and Line Number where this is called!
#define GET_CUR_CLASS_LINE (GET_CUR_CLASS + "(" + GET_CUR_LINE + ")")

#define V_LOG(LogCat, Param1)   UE_LOG(LogCat,Warning,TEXT("%s: %s"), *GET_CUR_CLASS_LINE, *FString(Param1))

#define COLLISION_WEAPON		ECC_GameTraceChannel1
#define COLLISION_PROJECTILE	ECC_GameTraceChannel2
#define COLLISION_PICKUP		ECC_GameTraceChannel3

#define MAX_PLAYER_NAME_LENGTH 16