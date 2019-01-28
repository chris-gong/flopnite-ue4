// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"

// Add default functionality here for any IMainMenuWidget functions that are not pure virtual.

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{

}

/** Retrieves FText value currently held in DisplayText */
FText UMainMenuWidget::GetCodeInput() const {
	return Code;

}

/** Assigns passed FText to DisplayText */
void UMainMenuWidget::SetCodeText(const FText& NewText) {
	Code = NewText;
}

bool UMainMenuWidget::LoadBattleRoyaleLevel() {
	UGameplayStatics::OpenLevel((UObject*)GetWorld(), FName(TEXT("Level_BattleRoyale")));
	return true;
}