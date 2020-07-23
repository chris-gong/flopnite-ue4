// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MaterialActor.generated.h"


class UBoxComponent;


UCLASS()
class FORTNITECLONE_API AMaterialActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMaterialActor();

	UPROPERTY()
		UBoxComponent * Box;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	int MaterialType; // 0 for wood, 1 for brick, 2 for steel

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	int MaterialCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Health")
		int Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Health")
		int MaxHealth;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
