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

	/* Player State properties */
	/*UPROPERTY(Replicated)
	bool InBuildMode;

	UPROPERTY(Replicated)
	FString BuildMode;

	UPROPERTY(Replicated)
	bool HoldingWeapon;

	UPROPERTY(Replicated)
	bool HoldingBandage;

	UPROPERTY(Replicated)
	bool AimedIn;

	UPROPERTY(Replicated)
	TArray<int> EquippedWeapons; //0 for pickaxe, 1 for assault rifle, 2 for shotgun

	UPROPERTY(Replicated)
	TArray<int> EquippedWeaponsAmmunition; //index 1 holds ammo for assault rifle, index 2 holds ammo for shotgun

	//below will be used primarily for keeping track of how many bullets in a clip while switching between guns and building
	UPROPERTY(Replicated)
	TArray<int> EquippedWeaponsClips; //index 1 holds current magazine clip count for assault rifle, index 2 holds current magazine clip count for shotgun

	UPROPERTY(Replicated)
	TArray<int> MaterialCounts; //index 0 holds count for wood, index 1 holds count for stone, index 2 holds count for steel

	UPROPERTY(Replicated)
	int CurrentWeapon; //0 for pickaxe, 1 for assault rifle, 2 for shotgun, -1 for non weapons like bandages

	UPROPERTY(Replicated)
	int BandageCount;

	UPROPERTY(Replicated)
	int KillCount;

	UPROPERTY(Replicated)
	bool JustShotRifle; // used to prevent player from spamming rifle

	UPROPERTY(Replicated)
	bool JustShotShotgun; // used to prevent player from spamming shotgun

	UPROPERTY(Replicated)
	bool JustSwungPickaxe; //used to prevent player from spamming pickaxe

	UPROPERTY(Replicated)
	bool JustUsedBandage; //used to prevent player from spamming bandage and doing other things while reloading

	UPROPERTY(Replicated)
	bool JustReloadedRifle; //used to prevent player from doing other things while reloading

	UPROPERTY(Replicated)
	bool JustReloadedShotgun; //used to prevent player from doing other things while reloading*/

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
	void Server_SetIsWalkingTrue();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetIsWalkingFalse();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetIsRunningTrue();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetIsRunningFalse();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetWalkingSpeed();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetRunningSpeed();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetAimedInSpeed();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetMovingLeft();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetMovingRight();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetMovingBackwards();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetMovingForwards();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ResetMovingRight();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ResetMovingForward();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetBuildModeWall();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetBuildModeRamp();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetBuildModeFloor();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_BuildStructures();

private:
	// Object creation can only happen after the character has finished being constructed
	virtual void BeginPlay() override;
};

