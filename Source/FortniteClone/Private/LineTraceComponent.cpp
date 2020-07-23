// Weekly - open-source on GitHub!


#include "LineTraceComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"

ULineTraceComponent::ULineTraceComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	
}



void ULineTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}

AActor* ULineTraceComponent::LineTraceSingle(FVector Start, FVector End)
{
	FHitResult HitResult;
	FCollisionObjectQueryParams CollisionParams;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	if(GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		Start,
		End,
		CollisionParams,
		CollisionQueryParams

	))
	{
		return HitResult.GetActor();
	}
	else
	{
		return nullptr;
	}
}

AActor* ULineTraceComponent::LineTraceSingleDebug(FVector Start, FVector End, bool ShowDebugLine)
{
	AActor* Actor = LineTraceSingle(Start, End);

	if (ShowDebugLine)
	{
		DrawDebugLine(
			GetWorld(),
			Start,
			End,
			FColor::Red,
			false,
			3.0f,
			0,
			5.0f
		);

	}
	
	return Actor;
}
