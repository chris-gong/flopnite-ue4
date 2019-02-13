// Created by YetiTech Studios.

#pragma once

#define CURRENT_CLASS								(FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) )
#define CURRENT_CLASS_FUNCTION							(FString(__FUNCTION__))
#define CURRENT_FUNCTION							(FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 ))
#define CURRENT_LINE_NUMBER							(FString::FromInt(__LINE__))
#define CURRENT_CLASS_WITH_LINE							("(" + CURRENT_LINE_NUMBER + ") " + CURRENT_CLASS)
#define CURRENT_FUNCTION_SIGNATURE						(FString(__FUNCSIG__))

#define NETMODE_WORLD (((GEngine == nullptr) || (GetWorld() == nullptr)) ? TEXT("[UNKNOWN_NETMODE] ") \
		: (GEngine->GetNetMode(GetWorld()) == NM_Client) ? TEXT("[Client] ") \
		: (GEngine->GetNetMode(GetWorld()) == NM_ListenServer) ? TEXT("[ListenServer] ") \
		: (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer) ? TEXT("[DedicatedServer] ") \
		: TEXT("[Standalone] "))

#define DEFINE_LOG(LogCategory)							DEFINE_LOG_CATEGORY_STATIC(LogCategory, All, All)

DEFINE_LOG(LogGameLiftClient)

#define CREATE_LOG_NORMAL(LogCategory, Param1)	   		UE_LOG(LogCategory, Log, TEXT("[%s::%s] %s"), *CURRENT_CLASS, *CURRENT_FUNCTION, *FString(Param1))
#define CREATE_LOG_VERBOSE(LogCategory, Param1)	   		UE_LOG(LogCategory, Verbose, TEXT("[%s::%s] %s"), *CURRENT_CLASS, *CURRENT_FUNCTION, *FString(Param1))
#define CREATE_LOG_VERYVERBOSE(LogCategory, Param1)		UE_LOG(LogCategory, VeryVerbose, TEXT("[%s::%s] %s"), *CURRENT_CLASS, *CURRENT_FUNCTION, *FString(Param1))
#define CREATE_LOG_WARNING(LogCategory, Param1)	   		UE_LOG(LogCategory, Warning, TEXT("[%s::%s] %s"), *CURRENT_CLASS, *CURRENT_FUNCTION, *FString(Param1))
#define CREATE_LOG_ERROR(LogCategory, Param1)	   		UE_LOG(LogCategory, Error, TEXT("[%s::%s] %s"), *CURRENT_CLASS, *CURRENT_FUNCTION, *FString(Param1))

#define LOG_NORMAL(Param1)	   							CREATE_LOG_NORMAL(LogGameLiftClient, Param1)
#define LOG_VERBOSE(Param1)	   							CREATE_LOG_VERBOSE(LogGameLiftClient, Param1)
#define LOG_VERYVERBOSE(Param1)							CREATE_LOG_VERYVERBOSE(LogGameLiftClient, Param1)
#define LOG_WARNING(Param1)	   							CREATE_LOG_WARNING(LogGameLiftClient, Param1)
#define LOG_ERROR(Param1)	   							CREATE_LOG_ERROR(LogGameLiftClient, Param1)
