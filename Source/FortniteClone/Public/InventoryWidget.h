// Weekly - open-source on GitHub!

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */

class UUserWidget;
class UTextReaderComponent;
class UButton;
class FEvent;
class UImage;

UCLASS(BlueprintType, Blueprintable)
class FORTNITECLONE_API UInventoryWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
	UImage * ImageSlot1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
		UImage * ImageSlot2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
		UImage * ImageSlot3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
		UImage * ImageSlot4;

	//MAX IS 5 IMAGES

private:

	virtual void NativeConstruct() override;

};
