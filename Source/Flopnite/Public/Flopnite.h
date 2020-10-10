// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#if WITH_EDITOR

    //Current Class
    #define FLOP_GET_CUR_CLASS (FString(FUNCTION).Left(FString(FUNCTION).Find(TEXT(":"))) )

    //Current Line Number
    #define FLOP_GET_CUR_LINE  (FString::FromInt(LINE))

    //Current Class and Line Number
    #define FLOP_GET_CUR_CLASS_LINE (GET_CUR_CLASS + "(" + GET_CUR_LINE + ")")


    #define FLOP_LOG(LogCat, Param1)   UE_LOG(LogCat,Warning,TEXT("%s: %s"), *GET_CUR_CLASS_LINE, *FString(Param1))

#else

    #if !WITH_EDITOR
        //Current Class
        #define FLOP_GET_CUR_CLASS

        //Current Line Number
        #define FLOP_GET_CUR_LINE

        //Current Class and Line Number
        #define FLOP_GET_CUR_CLASS_LINE


        #define FLOP_LOG(LogCat, Param1)
    #endif

#endif