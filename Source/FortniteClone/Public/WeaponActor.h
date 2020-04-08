// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FortDamageText.h"
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


UENUM(BlueprintType)
enum class EFortWeaponType : uint8
{
	WT_Common	UMETA(DisplayName = "Common"),
	WT_Uncommon	UMETA(DisplayName = "Uncommon"),
	WT_Rare	    UMETA(DisplayName = "Rare"),
	WT_Epic	    UMETA(DisplayName = "Epic"),
	WT_Legendary UMETA(DisplayName = "Legendary"),
	WT_Mythic UMETA(DisplayName = "Mythic")
};


USTRUCT(BlueprintType)
struct FWeaponInformation
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
		UTexture2D * WeaponImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
		EFortWeaponType WeaponType;
};


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	WEAPT_Rifle	UMETA(DisplayName = "Rifle"),
	WEAPT_Shotgun	UMETA(DisplayName = "Shotgun"),
	WEAPT_Pickaxe    UMETA(DisplayName = "Pickaxe")
};

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

	void Fire();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FWeaponInformation WeaponInfo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EWeaponType WeaponTypeEnum;


	// Called every frame
	virtual void Tick(float DeltaTime) override;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

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

	UPROPERTY(EditDefaultsOnly, Category = "WeaponType")
		int32 WeaponType; // 0 for pickaxe, 1 for assault rifle, 2 for shotgun

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponType")
		int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponType")
		int32 CurrentAmmo;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSound;

	/** Damage Text  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
		 TSubclassOf<AFortDamageText> DamageTextClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
		bool IsWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = Gameplay)
		float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = Gameplay, meta = (ClampMax = 200.0f))
		float MaxDamge;

	/** This is the damage that gets applied */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Gameplay)
		float ADamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
		FName AttachSocketName;

protected:
	
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


private:

	bool CanShoot();

	void UseAmmo();
};