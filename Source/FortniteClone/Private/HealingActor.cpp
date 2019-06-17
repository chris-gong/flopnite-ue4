// Fill out your copyright notice in the Description page of Project Settings.

#include "HealingActor.h"
#include "UnrealNetwork.h"

// Sets default values
AHealingActor::AHealingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHealingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealingActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHealingActor, Holder);
}