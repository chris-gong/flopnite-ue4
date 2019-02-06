// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FortniteCloneCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMyGame, Log, All);

class ABuildingActor;
class AWeaponActor;
class AHealingActor;
class AFortniteClonePlayerState;
class UThirdPersonAnimInstance;

UCLASS(config=Game)
class AFortniteCloneCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	class UCapsuleComponent* TriggerCapsule;

public:
	AFortniteCloneCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/* Class for wall preview actor */
	UPROPERTY(EditDefaultsOnly, Category = "Wall")
	TArray<TSubclassOf<ABuildingActor>> WallPreviewClasses;

	/* Class for wall preview actor */
	UPROPERTY(EditDefaultsOnly, Category = "Wall")
	TArray<TSubclassOf<ABuildingActor>> WallClasses;

	/* Class for wall preview actor */
	UPROPERTY(EditDefaultsOnly, Category = "Ramp")
	TArray<TSubclassOf<ABuildingActor>> RampPreviewClasses;

	/* Class for wall preview actor */
	UPROPERTY(EditDefaultsOnly, Category = "Ramp")
	TArray<TSubclassOf<ABuildingActor>> RampClasses;

	/* Class for wall preview actor */
	UPROPERTY(EditDefaultsOnly, Category = "Floor")
	TArray<TSubclassOf<ABuildingActor>> FloorPreviewClasses;

	/* Class for wall preview actor */
	UPROPERTY(EditDefaultsOnly, Category = "Floor")
	TArray<TSubclassOf<ABuildingActor>> FloorClasses;

	/* Array of weapon classes */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<TSubclassOf<AWeaponActor>> WeaponClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TSubclassOf<UThirdPersonAnimInstance> AnimInstanceClass;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	TSubclassOf<AFortniteClonePlayerState> PlayerStateClass;

	/* Class for healing actor */
	UPROPERTY(EditDefaultsOnly, Category = "Bandage")
	TSubclassOf<AHealingActor> BandageClass;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	UAnimMontage* RifleHipShootingAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	UAnimMontage* RifleIronsightsShootingAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	UAnimMontage* ShotgunHipShootingAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	UAnimMontage* ShotgunIronsightsShootingAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Pickaxe")
	UAnimMontage* PickaxeSwingingAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Healing")
	UAnimMontage* HealingAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Reload")
	UAnimMontage* RifleHipReloadAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Reload")
	UAnimMontage* ShotgunHipReloadAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Reload")
	UAnimMontage* RifleIronsightsReloadAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Reload")
	UAnimMontage* ShotgunIronsightsReloadAnimation;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Health")
	float Health;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth();

	UFUNCTION(BlueprintPure, Category = "Material")
	int GetWoodMaterialCount();

	UFUNCTION(BlueprintPure, Category = "Material")
	int GetStoneMaterialCount();

	UFUNCTION(BlueprintPure, Category = "Material")
	int GetSteelMaterialCount();

	UFUNCTION(BlueprintPure, Category = "Items")
	int GetAssaultRifleAmmoCount();

	UFUNCTION(BlueprintPure, Category = "Items")
	int GetShotgunAmmoCount();

	UFUNCTION(BlueprintPure, Category = "Items")
	int GetBandageCount();

	UFUNCTION(BlueprintPure, Category = "Kills")
	int GetKillCount();

	/* The current weapon being held */
	UPROPERTY(Replicated)
	AWeaponActor* CurrentWeapon;

	/* The current healing item being held */
	UPROPERTY(Replicated)
	AHealingActor* CurrentHealingItem;

	/* Anim instance properties */
	UPROPERTY(Replicated)
	bool IsRunning;

	UPROPERTY(Replicated)
	bool IsWalking;

	UPROPERTY(Replicated)
	float WalkingX;

	UPROPERTY(Replicated)
	float WalkingY;

	UPROPERTY(Replicated)
	float RunningX;

	UPROPERTY(Replicated)
	float RunningY;

	UPROPERTY(Replicated)
	bool HoldingWeapon;

	UPROPERTY(Replicated)
	bool AimedIn;

	UPROPERTY(Replicated)
	int HoldingWeaponType;

	UPROPERTY(Replicated)
	float AimPitch;

	UPROPERTY(Replicated)
	float AimYaw;

	UPROPERTY(Replicated)
	float InterpSpeed;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/* Start  and stop sprinting */
	UFUNCTION()
	void Sprint(float Value);

	/* Sets iswalking variable in anim instance to true */
	UFUNCTION()
	void StartWalking();

	/* Sets iswalking variable in anim instance to false */
	UFUNCTION()
	void StopWalking();

	UFUNCTION()
	TArray<float> CalculateWalkingXY();

	/* Show preview of where the wall will be built */
	UFUNCTION()
	void PreviewWall();

	/* Show preview of where the ramp will be built */
	UFUNCTION()
	void PreviewRamp();

	/* Show preview of where the floor will be built */
	UFUNCTION()
	void PreviewFloor();

	/* When the wall is shown, you will have the option to attempt to build it*/
	UFUNCTION()
	void BuildStructure();

	UFUNCTION()
	void SwitchBuildingMaterial();

	/* Set the animation variable as well as shoot a very small projectile from the gun or pickaxe*/
	UFUNCTION()
	void ShootGun();

	/* Set the animation variable as well as shoot a very small projectile from the gun*/
	UFUNCTION()
	void UseBandage();

	UFUNCTION()
	void AimGunIn();

	UFUNCTION()
	void AimGunOut();

	UFUNCTION()
	void Reload();

	UFUNCTION()
	void BandageTimeOut();

	UFUNCTION()
	void PickaxeTimeOut();

	UFUNCTION()
	void RifleTimeOut();

	UFUNCTION()
	void ShotgunTimeOut();

	UFUNCTION()
	void RifleReloadTimeOut();

	UFUNCTION()
	void ShotgunReloadTimeOut();

	UFUNCTION()
	void HoldPickaxe();

	UFUNCTION()
	void HoldAssaultRifle();

	UFUNCTION()
	void HoldShotgun();

	UFUNCTION()
	void HoldBandage();
	/* Current preview of wall to be built in build mode */

	/*UFUNCTION(Server, WithValidation)
	void ServerSetAnimInstance(UThirdPersonAnimInstance* AnimInstance);*/

	UPROPERTY(Replicated)
	ABuildingActor* BuildingPreview;

	/* Index of the class in array to spawn the weapon */
	UPROPERTY(Replicated)
	int CurrentWeaponType; // 0 for pickaxe, 1 for assault rifle, 2 for shotgun, -1 for non weapon items

	/* Index of the class in array to spawn structure */
	UPROPERTY(Replicated)
	int CurrentBuildingMaterial; // 0 for wood, 1 for stone, 2 for steel
	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	// Tick function is called every frame
	virtual void Tick(float DeltaTime) override;
	// Override playanimmontage to use pawn mesh
	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None) override;

	virtual void PostInitializeComponents() override;

	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;

	/* called when character touches something with its body */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetIsWalkingTrue();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetIsWalkingFalse();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetIsRunningTrue();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetIsRunningFalse();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetWalkingSpeed();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetRunningSpeed();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetAimedInSpeed();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetMovingLeft();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetMovingRight();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetMovingBackwards();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetMovingForwards();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerResetMovingRight();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerResetMovingForward();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetBuildModeWall();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetBuildModeRamp();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetBuildModeFloor();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBuildStructures();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFireWeapons();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReloadWeapons();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAimDownSights();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAimHipFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerShotgunTimeOut();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPickaxeTimeOut();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRifleTimeOut();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerShotgunReloadTimeOut();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRifleReloadTimeOut();

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticastCameraAimIn();

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticastCameraAimOut();

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticastPlayPickaxeSwingAnimation();

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticastPlayShootRifleAnimation();

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticastPlayShootShotgunAnimation();

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticastPlayShootRifleIronsightsAnimation();

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticastPlayShootShotgunIronsightsAnimation();

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticastPlayReloadRifleAnimation();

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticastPlayReloadRifleIronsightsAnimation();

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticastPlayReloadShotgunAnimation();

	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticastPlayReloadShotgunIronsightsAnimation();

private:
	// Object creation can only happen after the character has finished being constructed
	virtual void BeginPlay() override;
};

