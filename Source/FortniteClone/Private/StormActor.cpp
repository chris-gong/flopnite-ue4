// Fill out your copyright notice in the Description page of Project Settings.

#include "StormActor.h"
#include "FortniteCloneCharacter.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "FortniteCloneHUD.h"
#include "GameFramework/GameModeBase.h"
#include "FortniteCloneGameMode.h"
#include "Engine/Engine.h"

// Sets default values
AStormActor::AStormActor()
{
	bReplicates = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Damage = 1;
	IsShrinking = false;
	StormMoving = IsShrinking;
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
	timeElapsed = GetWorldTimerManager().GetTimerElapsed(StormDamageTimerHandle);
	timeRemaining = GetWorldTimerManager().GetTimerRemaining(StormDamageTimerHandle);

	if (HasAuthority()) {
		if (IsShrinking) {
			StormMoving = true;
			FVector NewScale = FVector(SizeScale.X * 0.999485, SizeScale.Y * 0.999485, SizeScale.Z);
			SizeScale = NewScale;
			SetActorScale3D(SizeScale);
		}
	}

}

void AStormActor::OnRep_StormMoving()
{
	OnStormMove.Broadcast();
}

FString AStormActor::GetStormTime()
{
	return FString::SanitizeFloat(timeRemaining);
}

bool AStormActor::GetStormStats()
{
	return IsShrinking;
}

void AStormActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AStormActor, Damage);
	DOREPLIFETIME(AStormActor, IsShrinking);
	DOREPLIFETIME(AStormActor, StormMoving);
	DOREPLIFETIME(AStormActor, SizeScale);
	DOREPLIFETIME(AStormActor, timeElapsed);
	DOREPLIFETIME(AStormActor, timeRemaining);
}

void AStormActor::ServerSetIsShrinking_Implementation() {
	if (Stage >= 4) {
		Stage = Stage + 1;
		if (Stage == 7) {
			// storm will now shrink for 3 stages (time of stage * 3)
			Stage = 0; 
			StormMoving = true;
			IsShrinking = !IsShrinking;
		}
	}
	/*
	if (Stage == 1) {
		Stage = Stage + 1;
		if (Stage == 1) {
			// storm will now shrink for 3 stages (time of stage * 3)
			Stage = 0;
			StormMoving = true;
			IsShrinking = !IsShrinking;
		}
	}
	if (Stage == 2) {
		Stage = Stage + 1;
		if (Stage == 2) {
			// storm will now shrink for 3 stages (time of stage * 3)
			Stage = 0;
			StormMoving = true;
			IsShrinking = !IsShrinking;
		}
	}
	if (Stage == 3) {
		Stage = Stage + 1;
		if (Stage == 3) {
			// storm will now shrink for 3 stages (time of stage * 3)
			Stage = 0;
			StormMoving = true;
			IsShrinking = !IsShrinking;
		}
	}
	if (Stage == 4) {
		Stage = Stage + 1;
		if (Stage == 4) {
			// storm will now shrink for 3 stages (time of stage * 3)
			Stage = 0;
			StormMoving = true;
			IsShrinking = !IsShrinking;
		}
	}
	if (Stage == 5) {
		Stage = Stage + 1;
		if (Stage == 5) {
			// storm will now shrink for 3 stages (time of stage * 3)
			Stage = 0;
			StormMoving = true;
			IsShrinking = !IsShrinking;
		}
	}
	if (Stage == 6) {
		Stage = Stage + 1;
		if (Stage == 6) {
			// storm will now shrink for 3 stages (time of stage * 3)
			Stage = 0;
			StormMoving = true;
			IsShrinking = !IsShrinking;
		}
	}
	if (Stage == 7) {
		Stage = Stage - 4;
		if (Stage == 7) {
			// storm will now shrink for 3 stages (time of stage * 3)
			Stage = 0;
			StormMoving = true;
			IsShrinking = !IsShrinking;
		}
	}
	*/
	else {
		Stage = Stage + 1;
		if (Stage == 4) {
			StormMoving = true;
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

void AStormActor::ServerStartStorm_Implementation(){

	GetWorldTimerManager().SetTimer(StormDamageTimerHandle, this, &AStormActor::ServerSetIsShrinking, 10.0f, true);
	GetWorldTimerManager().SetTimer(StormStateTimerHandle, this, &AStormActor::ServerSetNewDamage, 10.0f, true);



}

bool AStormActor::ServerStartStorm_Validate() {
	return true;
}
