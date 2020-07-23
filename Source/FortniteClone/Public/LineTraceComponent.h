// Weekly - open-source on GitHub!

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "DrawDebugHelpers.h"
//#include "Kismet/GameplayStatics.h"
//#include "Kismet/KismetSystemLibrary.h"
#include "LineTraceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORTNITECLONE_API ULineTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULineTraceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
		
		AActor * LineTraceSingle(FVector Start, FVector End);
		AActor * LineTraceSingleDebug(FVector Start, FVector End, bool ShowDebugLine);
};
