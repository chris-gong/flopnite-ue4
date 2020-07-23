// Weekly - open-source on GitHub!


#include "InventoryWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TextReaderComponent.h"
#include "Engine/Engine.h"
#include "Kismet\KismetMathLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"

UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ImageSlot1 = (UImage *)GetWidgetFromName(TEXT("ImageSlot1"));
	ImageSlot2 = (UImage *)GetWidgetFromName(TEXT("ImageSlot2"));
	ImageSlot3 = (UImage *)GetWidgetFromName(TEXT("ImageSlot3"));
	ImageSlot4 = (UImage *)GetWidgetFromName(TEXT("ImageSlot4"));
}
