// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StormActor.generated.h"



UCLASS()
class FORTNITECLONE_API AStormActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStormActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Replicated)
	float Damage;

	UPROPERTY(Replicated)
	bool IsShrinking;

	UPROPERTY(Replicated)
	FVector SizeScale;

	UPROPERTY()
	int Stage;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetIsShrinking();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetNewDamage();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartStorm();

};
