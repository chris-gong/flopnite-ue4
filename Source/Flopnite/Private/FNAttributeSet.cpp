// Fill out your copyright notice in the Description page of Project Settings.


#include "FNAttributeSet.h"
#include "Net/UnrealNetwork.h"

UFNAttributeSet::UFNAttributeSet() {

}

void UFNAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) {
	Super::PreAttributeChange(Attribute, NewValue);
}

void UFNAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
	Super::PostGameplayEffectExecute(Data);
}

void UFNAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UFNAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
}

void UFNAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFNAttributeSet, MoveSpeed, OldMoveSpeed);
}