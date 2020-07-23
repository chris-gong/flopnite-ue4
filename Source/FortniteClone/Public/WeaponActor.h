// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FortPickupActor.h"
#include "WeaponActor.generated.h"

class AProjectileActor;
class AFortniteCloneCharacter;
class UTexture2D;
class UAnimMontage;
class UAudioComponent;
class UParticleSystemComponent;
class UCameraShake;
class UForceFeedbackEffect;
class USoundCue;
class USkeletalMeshComponent;
class UStaticMeshComponent;
class UDamageType;
class UParticleSystem;
class AFortDamageText;
class USceneComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogWeaponActor, Log, All);

// Contains information of a single hitscan weapon linetrace
USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:

	UPROPERTY()
		TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY()
		FVector_NetQuantize TraceTo;
};



UCLASS()
class FORTNITECLONE_API AWeaponActor : public AFortPickupActor
{
	GENERATED_BODY()

public:
	
	AWeaponActor();

protected:

	virtual void BeginPlay() override;

public:

	void Fire();

	void Reload();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerReload();

	// Called every frame
	virtual void Tick(float DeltaTime) override;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
		USceneComponent* SceneComp;

	UPROPERTY(Replicated)
		AFortniteCloneCharacter* Holder;


	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	//associated bullet
	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
		TSubclassOf<AProjectileActor> BulletClass;

	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
		TSubclassOf<UDamageType> DamgeType;

	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
		int MagazineSize; // Only applies to assault rifle and shotgun

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Bullet", BlueprintReadOnly)
		int32 CurrentBulletCount; // Only applies to assault rifle and shotgun

	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponType")
		int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "WeaponType", meta = (ClampMax = 0.0f))
		int32 CurrentAmmo;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;

	/** Damage Text  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
		 TSubclassOf<AFortDamageText> DamageTextClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = Gameplay)
		float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = Gameplay, meta = (ClampMax = 200.0f))
		float MaxDamge;

	/** This is the damage that gets applied */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Gameplay)
		float ADamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int Bullets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
		FName AttachSocketName;

protected:

	UPROPERTY(Replicated)
	AFortDamageText * DamText;
	
	void SimulateWeaponFire();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void PlayFireEffects(FVector TraceEnd);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName MuzzleAttachPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName ShellEjectAttachPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName TracerTargetName;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFire();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UParticleSystem * TracerEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UParticleSystem* MuzzleFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UParticleSystem* ShellEjectFX;

	UPROPERTY(Transient)
		UParticleSystemComponent* MuzzlePSC;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UParticleSystem * ImpactEffect;

	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)
		FHitScanTrace HitScanTrace;

	UFUNCTION()
		void OnRep_HitScanTrace();

	UAudioComponent* PlayWeaponSound(USoundCue* SoundToPlay);

	float PlayWeaponAnimation(UAnimMontage* Animation, float InPlayRate = 1.0f, FName StartSectionName = NAME_None);

	void StopWeaponAnimation(UAnimMontage* Animation);

private:

	bool CanShoot();

	void UseAmmo();

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		USoundCue* FireSound;
};