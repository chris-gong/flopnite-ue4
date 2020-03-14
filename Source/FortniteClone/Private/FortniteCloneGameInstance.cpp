#include "FortniteCloneGameInstance.h"
#include "FortniteCloneHUD.h"
#include "Engine/GameInstance.h"

void UFortniteCloneGameInstance::Init()
{
	Super::Init();
}

void UFortniteCloneGameInstance::StormStarted()
{
	AFortniteCloneHUD * HUD = Cast<AFortniteCloneHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	HUD->CreateNewStormEye();
}

