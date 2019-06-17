// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileActor.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class AWeaponActor;
class AFortniteCloneCharacter;

UCLASS()
class FORTNITECLONE_API AProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	USphereComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

	/* called when projectile hits something */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	AWeaponActor* Weapon;

	AFortniteCloneCharacter* WeaponHolder;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Lifespan")
	float Lifespan; //used to distinguish range or projectile 

	UPROPERTY(EditDefaultsOnly, Category = "Speed")
	float ProjectileSpeed; //used to distinguish range or projectile 

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	int ProjectileType; // 0 for pickaxe, 1 for assault rifle, 2 for shotgun, will be used for determining the lifespan of the projectile

	UFUNCTION()
	void SelfDestruct();
};
