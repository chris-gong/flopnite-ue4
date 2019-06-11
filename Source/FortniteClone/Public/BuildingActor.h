// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingActor.generated.h"

UCLASS()
class FORTNITECLONE_API ABuildingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingActor();

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Health")
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = "Preview")
	bool IsPreview;

	UPROPERTY(EditDefaultsOnly, Category = "Type")
	int MaterialType; // 0 for wood, 1 for stone, 2 for steel

	UPROPERTY(EditDefaultsOnly, Category = "Type")
	FString StructureType; // Wall, Ramp, Floor

	UPROPERTY()
	int Id;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
