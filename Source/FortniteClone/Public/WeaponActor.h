// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

class AProjectileActor;
class AFortniteCloneCharacter;
class UTexture2D;

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

	UPROPERTY(Replicated)
	AFortniteCloneCharacter* Holder;

	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	int MagazineSize; // Only applies to assault rifle and shotgun

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Bullet")
	int CurrentBulletCount; // Only applies to assault rifle and shotgun

	UPROPERTY(EditDefaultsOnly, Category = "WeaponType")
	int WeaponType; // 0 for pickaxe, 1 for assault rifle, 2 for shotgun

	UPROPERTY(EditDefaultsOnly, Category = "WeaponType")
		 UTexture2D* WeaponImage; // 1 for assault rifle, 2 for shotgun, 3 for b, 4 for s

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
