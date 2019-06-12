// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UserWidget.h"
#include "TextReaderComponent.h"
#include "MainMenuWidget.generated.h"

class UUserWidget;
class UTextReaderComponent;

// This class does not need to be modified.
UCLASS(BlueprintType, Blueprintable)
class UMainMenuWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()

public:

	/** FText value displayed by this widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	FText Code;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reader")
	UTextReaderComponent* TextReader;

	/** Retrieves FText value currently held in DisplayText */
	UFUNCTION(BlueprintPure, Category = "Widgets|Text")
	FText GetCodeInput() const;

	UFUNCTION(BlueprintPure, Category = "Widgets|Text")
	void SetCodeText(const FText& gmt_NewDisplayText);

	UFUNCTION(BlueprintPure, Category = "Levels")
	bool LoadBattleRoyaleLevel();
};

