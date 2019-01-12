// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

class AProjectileActor;
class AFortniteCloneCharacter;

UCLASS()
class FORTNITECLONE_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//associated bullet
	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	TSubclassOf<AProjectileActor> BulletClass;

	AFortniteCloneCharacter* Holder;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage; // Only applies for pickaxe
};
