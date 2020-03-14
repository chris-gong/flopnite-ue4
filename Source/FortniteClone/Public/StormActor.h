// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StormActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStormMove);


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

	UPROPERTY(ReplicatedUsing = OnRep_StormMoving, Transient, BlueprintReadOnly, Category = "Delegats")
		bool StormMoving;

	UFUNCTION()
	void OnRep_StormMoving();

	UPROPERTY(BlueprintAssignable, Category = "Delegats")
		FOnStormMove OnStormMove;


	UPROPERTY(Replicated, BlueprintReadOnly)
	float Damage;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool IsShrinking;

	UPROPERTY(Replicated, BlueprintReadOnly)
	FVector SizeScale;

	UPROPERTY(BlueprintReadOnly)
	int Stage;

	FString TimeString;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetIsShrinking();

	FTimerHandle StormDamageTimerHandle;

	FTimerHandle StormStateTimerHandle;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float timeElapsed;
	UPROPERTY(Replicated, BlueprintReadOnly)
	float timeRemaining;

	UFUNCTION(BlueprintPure)
		FString GetStormTime();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetNewDamage();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartStorm();

	UFUNCTION(BlueprintPure)
		bool GetStormStats();

};
