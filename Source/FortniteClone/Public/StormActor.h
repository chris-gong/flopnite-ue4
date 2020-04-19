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

	void InitializeStorm();

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

	UFUNCTION(BlueprintPure)
	FString GetTimeRemainingUtillNextStage();

	UFUNCTION(BlueprintPure)
	FString GetTimeElapsedUtillNextStage();

	UPROPERTY(BlueprintAssignable, Category = "Delegats")
		FOnStormMove OnStormMove;

	UPROPERTY(BlueprintReadWrite)
		bool CanShrink = true;

	UPROPERTY(Replicated, BlueprintReadOnly)
		float timeElapsed;

	UPROPERTY(Replicated, BlueprintReadOnly)
		float timeRemaining;

	UPROPERTY()
		FTimerHandle StormStateTimerHandle;
		UPROPERTY()
		FTimerHandle StormSetupTimerHandle;

	UPROPERTY()
		FTimerHandle StormDamageTimerHandle;

	UPROPERTY(Replicated, BlueprintReadOnly)
	float Damage;

	UPROPERTY(Replicated, BlueprintReadWrite)
	bool IsShrinking;

	UPROPERTY(Replicated)
		float StormAdvanceStageRate;

	UPROPERTY(Replicated)
		float StormIncreaseDamageRate;

	UPROPERTY(Replicated, BlueprintReadOnly)
	FVector SizeScale;

	UPROPERTY(Replicated)
		FVector InitialSizeScale;

	UPROPERTY(Replicated)
		FVector InitialActorLocation;

	UPROPERTY(Replicated)
		TArray<FVector> SizeScales;

	UPROPERTY(Replicated)
		int32 ScaleIndex;

	UPROPERTY(Replicated)
		int32 ScaleTotalCount;

	UPROPERTY()
		float ScaleDownRate;

	UPROPERTY()
		float ScaleHighThreshold;

	UPROPERTY()
		float ScaleMidThreshold;

	UPROPERTY()
		float ScaleLowThreshold;

	UPROPERTY()
		float ScaleHighModifier;

	UPROPERTY()
		float ScaleMidModifier;

	UPROPERTY()
		float ScaleLowModifier;

	UPROPERTY(BlueprintReadOnly)
	int Stage;



	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}


	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartStorm();

	UFUNCTION(Server, Reliable, WithValidation)
		void AdvanceStage();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetNewDamage();

};
