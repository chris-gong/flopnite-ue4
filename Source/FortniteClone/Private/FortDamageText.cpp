// Weekly - open-source on GitHub!


#include "FortDamageText.h"
#include "Components/TextRenderComponent.h"

AFortDamageText::AFortDamageText()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComp->SetupAttachment(RootComponent);
	RootComponent = RootComp;

	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Damage Text"));
	Text->SetupAttachment(GetRootComponent());
}


void AFortDamageText::BeginPlay()
{
	Super::BeginPlay();
}


void AFortDamageText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

