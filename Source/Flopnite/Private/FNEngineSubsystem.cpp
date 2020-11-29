// Fill out your copyright notice in the Description page of Project Settings.


#include "FNEngineSubsystem.h"
#include "AbilitySystemGlobals.h"

void UFNEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UAbilitySystemGlobals::Get().InitGlobalData();
}
