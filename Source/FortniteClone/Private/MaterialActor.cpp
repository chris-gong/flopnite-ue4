// Fill out your copyright notice in the Description page of Project Settings.

#include "MaterialActor.h"
#include "UnrealNetwork.h"

// Sets default values
AMaterialActor::AMaterialActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMaterialActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMaterialActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMaterialActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMaterialActor, Health);
}