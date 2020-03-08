// Fill out your copyright notice in the Description page of Project Settings.

#include "StormActor.h"
#include "FortniteCloneCharacter.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "FortniteCloneHUD.h"
#include "Engine/Engine.h"

// Sets default values
AStormActor::AStormActor()
{
	bReplicates = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Damage = 1;
	IsShrinking = false;
	SizeScale = GetActorScale3D();
	Stage = 0; //Stages 0, 1, 2, 3, the circle is not shrinking, stage 4, 5, 6 the circle is shrinking and sets back to 0 afterwards
}


void AStormActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) {
		//move the storm to a random location on the map
		int32 X = FMath::RandRange(-7000 + 10000, 60000 - 10000);
		int32 Y = FMath::RandRange(-60000 + 10000, 17000 - 10000);
		SetActorLocation(FVector(X, Y, GetActorLocation().Z));

	}
}


void AStormActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority()) {
		if (IsShrinking) {
			FVector NewScale = FVector(SizeScale.X * 0.999485, SizeScale.Y * 0.999485, SizeScale.Z);
			SizeScale = NewScale;
			SetActorScale3D(SizeScale);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "current widget is not null");
			//AFortniteCloneHUD* HUD = Cast<AFortniteCloneHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
			//HUD->DrawNewEye();
		}
	}
}

void AStormActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AStormActor, Damage);
	DOREPLIFETIME(AStormActor, IsShrinking);//3000
	DOREPLIFETIME(AStormActor, SizeScale);
}

void AStormActor::ServerSetIsShrinking_Implementation() {
	if (Stage >= 4) {
		Stage = Stage + 1;
		if (Stage == 7) {
			// storm will now shrink for 3 stages (time of stage * 3)
			Stage = 0; 
			IsShrinking = !IsShrinking;
		}
	}
	else {
		Stage = Stage + 1;
		if (Stage == 4) {
			IsShrinking = !IsShrinking;
		}
	}
}

bool AStormActor::ServerSetIsShrinking_Validate() {
	return true;
}

void AStormActor::ServerSetNewDamage_Implementation() {
	Damage = Damage + 1;
}

bool AStormActor::ServerSetNewDamage_Validate() {
	return true;
}

void AStormActor::ServerStartStorm_Implementation() {
	FTimerHandle StormDamageTimerHandle;
	GetWorldTimerManager().SetTimer(StormDamageTimerHandle, this, &AStormActor::ServerSetIsShrinking, 30.0f, true);
	FTimerHandle StormStateTimerHandle;
	GetWorldTimerManager().SetTimer(StormStateTimerHandle, this, &AStormActor::ServerSetNewDamage, 240.0f, true);
}

bool AStormActor::ServerStartStorm_Validate() {
	return true;
}
