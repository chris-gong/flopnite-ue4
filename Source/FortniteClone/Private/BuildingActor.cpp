// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingActor.h"
#include "UnrealNetwork.h"
#include "Engine/Engine.h"

// Sets default values
ABuildingActor::ABuildingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABuildingActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABuildingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildingActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABuildingActor, Health);
}
