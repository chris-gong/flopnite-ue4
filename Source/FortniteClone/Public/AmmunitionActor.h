// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmunitionActor.generated.h"

UCLASS()
class FORTNITECLONE_API AAmmunitionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmunitionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Bullets")
	int BulletCount;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int WeaponType;
};
