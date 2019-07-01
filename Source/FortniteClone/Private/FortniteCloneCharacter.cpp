// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FortniteCloneCharacter.h"
#include "FortniteClonePlayerController.h"
#include "FortniteClonePlayerState.h"
#include "ThirdPersonAnimInstance.h"
#include "ProjectileActor.h"
#include "StormActor.h"
#include "FortniteCloneHUD.h"
#include "WeaponActor.h"
#include "BuildingActor.h"
#include "HealingActor.h"
#include "AmmunitionActor.h"
#include "Engine/Engine.h"
#include "UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Materials/Material.h"

DEFINE_LOG_CATEGORY(LogMyGame);
//////////////////////////////////////////////////////////////////////////
// AFortniteCloneCharacter

AFortniteCloneCharacter::AFortniteCloneCharacter()
{
	bReplicates = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Set up capsule component for detecting overlap
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(42.f, 96.0f);;
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AFortniteCloneCharacter::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AFortniteCloneCharacter::OnOverlapEnd);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CurrentWeaponType = 0;
	CurrentHealingItemType = -1;
	CurrentBuildingMaterial = 0;
	BuildingPreview = nullptr;

	// Animinstance properties
	IsWalking = false;
	IsRunning = false;
	HoldingWeapon = false;
	AimedIn = false;
	HoldingWeaponType = 0;
	AimPitch = 0.0;
	AimYaw = 0.0;
	InterpSpeed = 15.0; //change this to set aim sensitivity
	WalkingX = 0;
	WalkingY = 0;
	RunningX = 0;
	RunningY = 0;
	InStorm = true;
	CurrentStructureId = 0;	

	// Playerstate properties
	/*InBuildMode = false;
	BuildMode = FString("None");
	HoldingWeapon = false;
	HoldingHealingItem = false;
	AimedIn = false;
	EquippedWeapons.Add(0); //pickaxe
	EquippedWeaponsAmmunition.Add(0); // pickaxe
	EquippedWeaponsAmmunition.Add(0); // assault rifle
	EquippedWeaponsAmmunition.Add(0); // shotgun
	EquippedWeaponsClips.Add(0); // pickaxe
	EquippedWeaponsClips.Add(0); // assault rifle
	EquippedWeaponsClips.Add(0); // shotgun
	MaterialCounts.Add(0); // wood
	MaterialCounts.Add(0); // stone
	MaterialCounts.Add(0); // steel
	CurrentWeapon = 0;
	JustShotRifle = false;
	JustShotShotgun = false;
	JustSwungPickaxe = false;
	JustUsedHealingItem = false;
	JustReloadedRifle = false;
	JustReloadedShotgun = false;
	KillCount = 0;*/

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

}

//////////////////////////////////////////////////////////////////////////
// Input

void AFortniteCloneCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFortniteCloneCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFortniteCloneCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Sprint", this, &AFortniteCloneCharacter::Sprint);

	PlayerInputComponent->BindAction("Walk", IE_Pressed, this, &AFortniteCloneCharacter::StartWalking);
	PlayerInputComponent->BindAction("Walk", IE_Released, this, &AFortniteCloneCharacter::StopWalking);
	PlayerInputComponent->BindAction("PreviewWall", IE_Pressed, this, &AFortniteCloneCharacter::PreviewWall);
	PlayerInputComponent->BindAction("PreviewRamp", IE_Pressed, this, &AFortniteCloneCharacter::PreviewRamp);
	PlayerInputComponent->BindAction("PreviewFloor", IE_Pressed, this, &AFortniteCloneCharacter::PreviewFloor);
	PlayerInputComponent->BindAction("SwitchBuildingMaterial", IE_Pressed, this, &AFortniteCloneCharacter::SwitchBuildingMaterial);
	PlayerInputComponent->BindAction("ShootGun", IE_Pressed, this, &AFortniteCloneCharacter::ShootGun);
	PlayerInputComponent->BindAction("UseHealingItem", IE_Pressed, this, &AFortniteCloneCharacter::UseHealingItem);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFortniteCloneCharacter::Reload);
	PlayerInputComponent->BindAction("HoldPickaxe", IE_Pressed, this, &AFortniteCloneCharacter::HoldPickaxe);
	PlayerInputComponent->BindAction("HoldAssaultRifle", IE_Pressed, this, &AFortniteCloneCharacter::HoldAssaultRifle);
	PlayerInputComponent->BindAction("HoldShotgun", IE_Pressed, this, &AFortniteCloneCharacter::HoldShotgun);
	PlayerInputComponent->BindAction("HoldBandage", IE_Pressed, this, &AFortniteCloneCharacter::HoldBandage);
	PlayerInputComponent->BindAction("HoldPotion", IE_Pressed, this, &AFortniteCloneCharacter::HoldPotion);
	PlayerInputComponent->BindAction("Ironsights", IE_Pressed, this, &AFortniteCloneCharacter::AimGunIn);
	PlayerInputComponent->BindAction("Ironsights", IE_Released, this, &AFortniteCloneCharacter::AimGunOut);
	PlayerInputComponent->BindAction("OpenSettings", IE_Pressed, this, &AFortniteCloneCharacter::OpenSettingsMenu);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFortniteCloneCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFortniteCloneCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("BuildStructure", this, &AFortniteCloneCharacter::BuildStructure);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AFortniteCloneCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AFortniteCloneCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AFortniteCloneCharacter::OnResetVR);

}

void AFortniteCloneCharacter::BeginPlay() {
	Super::BeginPlay();
	//GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Client ")  + FString::FromInt(ENetMode::NM_Client) + FString(" server ") + FString::FromInt(ENetMode::NM_DedicatedServer));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(GetNetMode()));
	/*if (GetNetMode() != ENetMode::NM_Client || GetNetMode() != ENetMode::NM_Standalone) {
		return;
	}*/
	if (HasAuthority()) {
		if (WeaponClasses[CurrentWeaponType]) {
			// for some reason I can't call clientgetweapontransform here
			FName WeaponSocketName = TEXT("hand_right_socket_pickaxe");
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
			CurrentWeapon = GetWorld()->SpawnActor<AWeaponActor>(WeaponClasses[CurrentWeaponType], GetActorLocation(), GetActorRotation());
			UStaticMeshComponent* WeaponStaticMeshComponent = Cast<UStaticMeshComponent>(CurrentWeapon->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			WeaponStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, WeaponSocketName);
			CurrentWeapon->Holder = this;
			HoldingWeapon = true;
			AimedIn = false;
			HoldingWeaponType = 1;
		}
		// find the storm and keep a reference to it for damage purposes
		TArray<AActor*> StormActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStormActor::StaticClass(), StormActors);
		if (StormActors.Num() > 0) {
			if (StormActors[0] != nullptr) {
				CurrentStorm = Cast<AStormActor>(StormActors[0]);
			}
		}
		// set skin
		if (GetMesh()) {
			MaterialOrMaterialInstance = FMath::RandRange(0, 1);
			CurrentSkin = FMath::RandRange(0, 1);

			USkeletalMeshComponent* CharacterMesh = GetMesh();
			if (MaterialOrMaterialInstance == 0) {
				// assign UMaterial
				CharacterMesh->SetMaterial(0, SkinMaterials[CurrentSkin]);
			}
			else {
				// assign UMaterialInstance
				CharacterMesh->SetMaterial(0, SkinMaterialInstances[CurrentSkin]);
			}
			SkinInitialized = true;
		}
		
		FTimerHandle StormDamageTimerHandle;
		GetWorldTimerManager().SetTimer(StormDamageTimerHandle, this, &AFortniteCloneCharacter::ServerApplyStormDamage, 1.0f, true);

	}
}

void AFortniteCloneCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		// Objects only replicate from server to client. If we didn't guard this
		// the client would create the object just fine but it would get replaced
		// by the server version (more accurately the property would be replaced to
		// point to the version from the server. The one the client allocated would
		// eventually be garbage collected.
		//AnimInstance = NewObject<UThirdPersonAnimInstance>(this); // NOTE: Very important, objects Outer must be our Actor!
	}
}

bool AFortniteCloneCharacter::ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	/*if (AnimInstance != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("replicated ") + FString::FromInt(GetNetMode()));
		WroteSomething |= Channel->ReplicateSubobject(AnimInstance, *Bunch, *RepFlags);
	}
	if (State != nullptr) {
		WroteSomething |= Channel->ReplicateSubobject(State, *Bunch, *RepFlags);
	}
	if (BuildingPreview != nullptr) {
		WroteSomething |= Channel->ReplicateSubobject(BuildingPreview, *Bunch, *RepFlags);
	}*/
	return WroteSomething;
}

void AFortniteCloneCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFortniteCloneCharacter, BuildingPreview);
	DOREPLIFETIME(AFortniteCloneCharacter, CurrentBuildingMaterial);
	DOREPLIFETIME(AFortniteCloneCharacter, CurrentHealingItem);
	DOREPLIFETIME(AFortniteCloneCharacter, CurrentHealingItemType);
	DOREPLIFETIME(AFortniteCloneCharacter, CurrentWeapon);
	DOREPLIFETIME(AFortniteCloneCharacter, CurrentStorm);
	DOREPLIFETIME(AFortniteCloneCharacter, CurrentWeaponType);
	DOREPLIFETIME(AFortniteCloneCharacter, Health);
	DOREPLIFETIME(AFortniteCloneCharacter, Shield);

	DOREPLIFETIME(AFortniteCloneCharacter, IsWalking);
	DOREPLIFETIME(AFortniteCloneCharacter, IsRunning);
	DOREPLIFETIME(AFortniteCloneCharacter, WalkingX);
	DOREPLIFETIME(AFortniteCloneCharacter, WalkingY);
	DOREPLIFETIME(AFortniteCloneCharacter, RunningX);
	DOREPLIFETIME(AFortniteCloneCharacter, RunningY);
	DOREPLIFETIME(AFortniteCloneCharacter, HoldingWeapon);
	DOREPLIFETIME(AFortniteCloneCharacter, HoldingWeaponType);
	DOREPLIFETIME(AFortniteCloneCharacter, AimedIn);
	DOREPLIFETIME(AFortniteCloneCharacter, AimPitch);
	DOREPLIFETIME(AFortniteCloneCharacter, AimYaw);
	DOREPLIFETIME(AFortniteCloneCharacter, InterpSpeed);
	DOREPLIFETIME(AFortniteCloneCharacter, InStorm);
	DOREPLIFETIME(AFortniteCloneCharacter, SkinInitialized);
	DOREPLIFETIME(AFortniteCloneCharacter, MaterialOrMaterialInstance);
	DOREPLIFETIME(AFortniteCloneCharacter, CurrentSkin);
}

void AFortniteCloneCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Tick mode ") + FString::FromInt(GetNetMode()));
	FVector DirectionVector = FVector(0, AimYaw, AimPitch);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Tick mode ") + FString::SanitizeFloat(DirectionVector.Z));
	if (HasAuthority()) {
		if (GetController()) {
			AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
			if (State) {
				if (State->InBuildMode && State->BuildMode == FString("Wall")) {
					if (BuildingPreview) {
						BuildingPreview->Destroy(); //destroy the last wall preview
					}
					FVector ProjectedLocation = GetActorLocation() + (GetActorForwardVector() * FVector(200.0, 50.0, 1.0)) + (FVector(0, 0, DirectionVector.Z) * 2.0); // projected location before setting to grid using actor forward and direction (aim offset) vectors
					float GridLocationX = FMath::RoundHalfFromZero(ProjectedLocation.X / 480.00002) * 480;
					float GridLocationY = FMath::RoundHalfFromZero(ProjectedLocation.Y / 480.00002) * 480;
					float GridLocationZ = FMath::RoundHalfFromZero(ProjectedLocation.Z / 480.00002) * 480;
					FVector GridLocation = FVector(GridLocationX, GridLocationY, GridLocationZ);
					FRotator ProjectedRotation = GetActorRotation().Add(0, 90, 0); // projected rotation before setting to grid using the actor's rotation
					float GridRotationYaw = FMath::RoundHalfFromZero(ProjectedRotation.Yaw / 90.0) * 90;

					FRotator GridRotation = FRotator(0, GridRotationYaw, 0);

					if (FMath::Abs(GridRotationYaw) == 0) {
						GridLocation.Y -= 240.00001;
						if (ProjectedLocation.X < GridLocation.X) {
							GridLocation.X -= 240.00001;
						}
						else {
							GridLocation.X += 240.00001;
						}
					}
					if (FMath::Abs(GridRotationYaw) == 180) {
						GridLocation.Y += 240.00001;
						if (ProjectedLocation.X > GridLocation.X) {
							GridLocation.X += 240.00001;
						}
						else {
							GridLocation.X -= 240.00001;
						}
					}

					//LogMsg = FString("Current building material ") + FString::FromInt(CurrentBuildingMaterial) + FString(" ") + FString::SanitizeFloat(GridRotationYaw) + FString(" ") + FString::SanitizeFloat(ProjectedRotation.Yaw);
					//UE_LOG(LogMyGame, Warning, TEXT("%s"), *LogMsg);
					if (CurrentBuildingMaterial >= 0 && CurrentBuildingMaterial <= 2) {
						if (WallPreviewClasses.IsValidIndex(CurrentBuildingMaterial)) {
							if (WallPreviewClasses[CurrentBuildingMaterial] != nullptr) {
								BuildingPreview = GetWorld()->SpawnActor<ABuildingActor>(WallPreviewClasses[CurrentBuildingMaterial], GridLocation, GridRotation); //set the new wall preview
							}
						}
					}
				}
				if (State->InBuildMode && State->BuildMode == FString("Ramp")) {
					if (BuildingPreview) {
						BuildingPreview->Destroy(); //destroy the last wall preview
					}
					// TODO print out directionvector.z (looking straight forward vs up and down affects x value)
					// TODO check why the overlapping function when building is not working
					//original 400, 200, 2
					FVector ProjectedLocation = GetActorLocation() + (GetActorForwardVector() * FVector(50.0, 6.25, 1.0)) + (FVector(0, 0, DirectionVector.Z) * 1.5); // projected location before setting to grid using actor forward and direction (aim offset) vectors
					float GridLocationX = FMath::RoundHalfFromZero(ProjectedLocation.X / 480.00002) * 480;
					float GridLocationY = FMath::RoundHalfFromZero(ProjectedLocation.Y / 480.00002) * 480;
					float GridLocationZ = FMath::RoundHalfFromZero(ProjectedLocation.Z / 480.00002) * 480;
					FVector GridLocation = FVector(GridLocationX, GridLocationY, GridLocationZ);
					FRotator ProjectedRotation = GetActorRotation().Add(0, 90, 0); // projected rotation before setting to grid using the actor's rotation
					float GridRotationYaw = FMath::RoundHalfFromZero(ProjectedRotation.Yaw / 90.0) * 90;

					FRotator GridRotation = FRotator(0, GridRotationYaw, 0);

					if (FMath::Abs(GridRotationYaw) == 0) {
						GridLocation.Y -= 240.00001;
						if (ProjectedLocation.X < GridLocation.X) {
							GridLocation.X -= 240.00001;
						}
						else {
							GridLocation.X += 240.00001;
						}
					}
					if (FMath::Abs(GridRotationYaw) == 180) {
						GridLocation.Y += 240.00001;
						if (ProjectedLocation.X > GridLocation.X) {
							GridLocation.X += 240.00001;
						}
						else {
							GridLocation.X -= 240.00001;
						}
					}

					/*LogMsg = FString("Current building material ") + FString::FromInt(CurrentBuildingMaterial) + FString(" ") + FString::SanitizeFloat(GridRotationYaw) + FString(" ") + FString::SanitizeFloat(ProjectedRotation.Yaw);
					UE_LOG(LogMyGame, Warning, TEXT("%s"), *LogMsg);*/
					if (CurrentBuildingMaterial >= 0 && CurrentBuildingMaterial <= 2) {
						if (RampPreviewClasses.IsValidIndex(CurrentBuildingMaterial)) {
							if (RampPreviewClasses[CurrentBuildingMaterial] != nullptr) {
								BuildingPreview = GetWorld()->SpawnActor<ABuildingActor>(RampPreviewClasses[CurrentBuildingMaterial], GridLocation, GridRotation); //set the new ramp preview
							}
						}
					}
				}
				if (State->InBuildMode && State->BuildMode == FString("Floor")) {
					if (BuildingPreview) {
						BuildingPreview->Destroy(); //destroy the last wall preview
					}

					FVector ProjectedLocation = GetActorLocation() + (GetActorForwardVector() * FVector(100.0, 25.0, 1.0)) + (FVector(0, 0, DirectionVector.Z) * 2.0); // projected location before setting to grid using actor forward and direction (aim offset) vectors
					float GridLocationX = FMath::RoundHalfFromZero(ProjectedLocation.X / 480.00002) * 480;
					float GridLocationY = FMath::RoundHalfFromZero(ProjectedLocation.Y / 480.00002) * 480;
					float GridLocationZ = FMath::RoundHalfFromZero(ProjectedLocation.Z / 480.00002) * 480;
					FVector GridLocation = FVector(GridLocationX, GridLocationY, GridLocationZ);
					FRotator ProjectedRotation = GetActorRotation().Add(0, 90, 0); // projected rotation before setting to grid using the actor's rotation
					float GridRotationYaw = FMath::RoundHalfFromZero(ProjectedRotation.Yaw / 90.0) * 90;

					FRotator GridRotation = FRotator(0, GridRotationYaw, 0);

					if (FMath::Abs(GridRotationYaw) == 0) {
						GridLocation.Y -= 240.00001;
						if (ProjectedLocation.X < GridLocation.X) {
							GridLocation.X -= 240.00001;
						}
						else {
							GridLocation.X += 240.00001;
						}
					}
					if (FMath::Abs(GridRotationYaw) == 180) {
						GridLocation.Y += 240.00001;
						if (ProjectedLocation.X > GridLocation.X) {
							GridLocation.X += 240.00001;
						}
						else {
							GridLocation.X -= 240.00001;
						}
					}

					/*FString LogMsg = FString("Current building material ") + FString::FromInt(CurrentBuildingMaterial);
					UE_LOG(LogMyGame, Warning, TEXT("%s"), *LogMsg);*/
					if (CurrentBuildingMaterial >= 0 && CurrentBuildingMaterial <= 2) {
						if (FloorPreviewClasses.IsValidIndex(CurrentBuildingMaterial)) {
							if (FloorPreviewClasses[CurrentBuildingMaterial] != nullptr) {
								BuildingPreview = GetWorld()->SpawnActor<ABuildingActor>(FloorPreviewClasses[CurrentBuildingMaterial], GridLocation, GridRotation); //set the new floor preview
							}
						}
					}
				}
			}
		}
		FRotator ControlRotation = GetControlRotation();
		FRotator ActorRotation = GetActorRotation();

		FRotator DeltaRotation = ControlRotation - ActorRotation;
		DeltaRotation.Normalize();

		FRotator AimRotation = FRotator(AimPitch, AimYaw, 0);
		FRotator InterpolatedRotation = FMath::RInterpTo(AimRotation, DeltaRotation, DeltaTime, InterpSpeed);

		float NewPitch = FMath::ClampAngle(InterpolatedRotation.Pitch, -90, 90);
		float NewYaw = FMath::ClampAngle(InterpolatedRotation.Yaw, -90, 90);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(NewPitch).Append(FString::FromInt(NewYaw)));
		AimPitch = NewPitch;
		AimYaw = NewYaw;
	}
}

void AFortniteCloneCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (HasAuthority()) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("NetMode: ") + FString::FromInt(GetNetMode()) + FString(" Player overlapped with: ") + OtherActor->GetName());
		if (OtherActor != nullptr && OtherActor != this) {
			if (CurrentWeapon != nullptr && OtherActor == (AActor*)CurrentWeapon) {
				// if the character is overlapping with its weapon, dont do anything about it
				return;
			}
			if (CurrentHealingItem != nullptr && OtherActor == (AActor*)CurrentHealingItem) {
				// if the character is overlapping with its healing item, dont do anything about it
				return;
			}
			if (OtherActor->IsA(AWeaponActor::StaticClass())) {
				AWeaponActor* WeaponActor = Cast<AWeaponActor>(OtherActor);
				if (WeaponActor->WeaponType == 0) {
					return; // do nothing if it's a pickaxe
				}
				if (WeaponActor->Holder != nullptr) {
					return; // do nothing if someone is holding the weapon
				}
				if (GetController()) {
					// pick up the item if the two conditions above are false
					AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
					if (State) {
						if (State->InBuildMode || State->JustShotRifle || State->JustShotShotgun || State->JustSwungPickaxe || State->JustUsedHealingItem || State->JustReloadedRifle || State->JustReloadedShotgun) {
							return; // can't pick up items while in build mode or if just shot rifle, shot shotgun, swung pickaxe, used healing item, or reloaded
						}
						// if the player already has a weapon of this type, do not equip it
						if (State->EquippedWeapons.Contains(WeaponActor->WeaponType)) {
							return;
						}
						// Destroy old weapon/healing item
						if (CurrentWeapon && CurrentWeaponType > 0 && CurrentWeaponType < 3) {
							State->EquippedWeaponsClips[CurrentWeaponType] = CurrentWeapon->CurrentBulletCount;
						}
						if (CurrentWeapon) {
							CurrentWeapon->Destroy();
							CurrentWeapon = nullptr;
						}
						if (CurrentHealingItem) {
							CurrentHealingItem->Destroy();
							CurrentHealingItem = nullptr;
						}
						// PICK UP WEAPON
						State->EquippedWeapons.Add(WeaponActor->WeaponType);
						State->EquippedWeaponsClips[WeaponActor->WeaponType] = WeaponActor->MagazineSize; // this has to be done before calling client method below
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("MagSize: ") + FString::FromInt(WeaponActor->MagazineSize));
						ClientGetWeaponTransform(WeaponActor->WeaponType);

						WeaponActor->Destroy();
					}

				}

			}
			else if (OtherActor->IsA(AHealingActor::StaticClass())) {
				//pick up the item
				AHealingActor* HealingActor = Cast<AHealingActor>(OtherActor);
				AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
				if (CurrentHealingItem) {
					CurrentHealingItem->Destroy();
					CurrentHealingItem = nullptr;
				}
				if (State) {
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, OtherActor->GetName());
					if (State->InBuildMode || State->JustShotRifle || State->JustShotShotgun || State->JustSwungPickaxe || State->JustUsedHealingItem || State->JustReloadedRifle || State->JustReloadedShotgun) {
						return; // can't pick up items while in build mode or if just shot rifle, shot shotgun, swung pickaxe, used healing item, or reloaded
					}
					//CurrentHealingItem = Cast<AHealingActor>(OtherActor);
					if (HealingActor->Holder != nullptr) {
						return; // do nothing if someone is holding the healing item
					}
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("didn't end early"));
					// Destroy old weapon
					if (CurrentWeapon && CurrentWeaponType > 0 && CurrentWeaponType < 3) {
						State->EquippedWeaponsClips[CurrentWeaponType] = CurrentWeapon->CurrentBulletCount;
					}
					if (CurrentWeapon) {
						CurrentWeapon->Destroy();
						CurrentWeapon = nullptr;
					}
					// pick up healing item
					State->HealingItemCounts[HealingActor->HealingType] += HealingActor->Count;
					ClientGetHealingItemTransform(HealingActor->HealingType);
					HealingActor->Destroy();
				}
			}
			else if (OtherActor->IsA(AAmmunitionActor::StaticClass())) {
				if (GetController()) {
					AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
					AAmmunitionActor* Ammo = Cast<AAmmunitionActor>(OtherActor);
					if (State) {
						// increment ammo count
						State->EquippedWeaponsAmmunition[Ammo->WeaponType] += Ammo->BulletCount;
					}
					Ammo->Destroy();
				}
			}
			else if (OtherActor->IsA(AStormActor::StaticClass())) {
				/*FString LogMsg = FString("storm overlap begin ") + FString::FromInt(GetNetMode());
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, LogMsg);
				UE_LOG(LogMyGame, Warning, TEXT("%s"), *LogMsg);*/
				InStorm = false;
			}
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, OtherActor->GetName());
		}
	}
}

void AFortniteCloneCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (HasAuthority()) {
		/*FString LogMsg = FString("storm overlap end ") + FString::FromInt(GetNetMode());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, LogMsg);
		UE_LOG(LogMyGame, Warning, TEXT("%s"), *LogMsg);*/
		if (OtherActor != nullptr) {
			if (OtherActor == this) {
				return;
			}
			if (OtherActor->IsA(AStormActor::StaticClass())) {
				InStorm = true;
			}
		}
	}
}

float AFortniteCloneCharacter::PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	USkeletalMeshComponent* UseMesh = GetMesh();
	if (AnimMontage && UseMesh && UseMesh->AnimScriptInstance)
	{
		return UseMesh->AnimScriptInstance->Montage_Play(AnimMontage, InPlayRate);
	}

	return 0.0f;
}

void AFortniteCloneCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AFortniteCloneCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AFortniteCloneCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AFortniteCloneCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFortniteCloneCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFortniteCloneCharacter::MoveForward(float Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("move forward ") + FString::FromInt(GetNetMode()));
	
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(Controller->PlayerState);
		if (State) {
			if (State->JustUsedHealingItem) {
				return;
			}
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			if (AimedIn) {
				AddMovementInput(Direction, Value * 0.2);
			}
			else if (IsRunning) {
				AddMovementInput(Direction, Value * 0.9);
			}
			else if (IsWalking) {
				AddMovementInput(Direction, Value * 0.45);
			}
		}
	}
	/*UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
	if (Animation) {
		//set blend space variable
		Animation->WalkingY = Value * 90;
		Animation->RunningY = Value * 90;
		//Server_SetMovingVariables();
	}*/
	if (Value == 0) {
		ServerResetMovingForward();
	}
	else if (Value > 0) {
		ServerSetMovingForwards();
	}
	else {
		ServerSetMovingBackwards();
	}
}

void AFortniteCloneCharacter::MoveRight(float Value)
{

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(Controller->PlayerState);
		if (State) {
			if (State->JustUsedHealingItem) {
				return;
			}
		}
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		if (AimedIn) {
			AddMovementInput(Direction, Value * 0.2);
		}
		else if (IsRunning) {
			AddMovementInput(Direction, Value * 0.9);
		}
		else if (IsWalking) {
			AddMovementInput(Direction, Value * 0.45);
		}
	}
	/*UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
	if (Animation) {
		//set blend space variable
		Animation->WalkingX = Value * 90;
		Animation->RunningX = Value * 90;
		//Server_SetMovingVariables();
	}*/
	if (Value == 0) {
		ServerResetMovingRight();
	}
	else if (Value > 0) {
		ServerSetMovingRight();
	}
	else {
		ServerSetMovingLeft();
	}
}

void AFortniteCloneCharacter::Sprint(float Value) {
	APlayerController* LocalController = Cast<APlayerController>(GetController());
	bool ADown = LocalController->IsInputKeyDown(EKeys::A);
	bool WDown = LocalController->IsInputKeyDown(EKeys::W);
	bool SDown = LocalController->IsInputKeyDown(EKeys::S);
	bool DDown = LocalController->IsInputKeyDown(EKeys::D);
	bool OnlyAOrDDown = !WDown && !SDown && (ADown || DDown);
	UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
	if (Animation) {
		if (Animation->AimedIn) {
			//ServerSetAimedInSpeed();
		}
		else if (Value == 0) {
			//ServerSetWalkingSpeed();
			ServerSetIsRunningFalse();
		}
		else {
			// can only sprint if the w key is held down by itself or in combination with the a or d keys
			if (!(OnlyAOrDDown || SDown) && WDown) {
				//ServerSetRunningSpeed();
				ServerSetIsRunningTrue();
			}
			else {
				//ServerSetWalkingSpeed();
				ServerSetIsRunningFalse();
			}
		}
	}
	//Server_SetAnimationVariables();
}

void AFortniteCloneCharacter::StartWalking() {
	ServerSetIsWalkingTrue();
}


void AFortniteCloneCharacter::StopWalking() {
	//UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
	APlayerController* LocalController = Cast<APlayerController>(GetController());
	bool ADown = LocalController->IsInputKeyDown(EKeys::A);
	bool WDown = LocalController->IsInputKeyDown(EKeys::W);
	bool SDown = LocalController->IsInputKeyDown(EKeys::S);
	bool DDown = LocalController->IsInputKeyDown(EKeys::D);
	bool NoWalkingKeysDown = !ADown && !WDown && !SDown && !DDown;

	if (NoWalkingKeysDown) {
		ServerSetIsWalkingFalse();
	}
	//ServerSetAnimationVariables();
}

TArray<float> AFortniteCloneCharacter::CalculateWalkingXY() {
	float X = 0;
	float Y = 0;
	APlayerController* LocalController = Cast<APlayerController>(GetController());
	if (LocalController->IsInputKeyDown(EKeys::A)) {
		X -= 90;
	}
	if (LocalController->IsInputKeyDown(EKeys::D)) {
		X += 90;
	}
	if (LocalController->IsInputKeyDown(EKeys::W)) {
		Y += 90;
	}
	if (LocalController->IsInputKeyDown(EKeys::S)) {
		Y -= 90;
	}
	TArray<float> Coords;
	Coords.Add(X);
	Coords.Add(Y);
	return Coords;
}

void AFortniteCloneCharacter::PreviewWall() {
	ServerSetBuildModeWall();
}

void AFortniteCloneCharacter::PreviewRamp() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "c key pressed");
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "c key presse2d");
	ServerSetBuildModeRamp();
}

void AFortniteCloneCharacter::PreviewFloor() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "f key pressed");
	ServerSetBuildModeFloor();
}

void AFortniteCloneCharacter::BuildStructure(float Value) {
	if (Value > 0) {
		// Build a structure on the client side first then run a server rpc that destroys that structure and respawns it
		if (GetController()) {
			AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
			if (State) {
				FVector DirectionVector = FVector(0, AimYaw, AimPitch);
				if (State->InBuildMode && State->BuildMode == FString("Wall") && State->MaterialCounts[CurrentBuildingMaterial] >= 10) {
					TArray<AActor*> OverlappingActors;

					FVector ProjectedLocation = GetActorLocation() + (GetActorForwardVector() * FVector(200.0, 50.0, 1.0)) + (FVector(0, 0, DirectionVector.Z) * 2.0); // projected location before setting to grid using actor forward and direction (aim offset) vectors
					float GridLocationX = FMath::RoundHalfFromZero(ProjectedLocation.X / 480.00002) * 480;
					float GridLocationY = FMath::RoundHalfFromZero(ProjectedLocation.Y / 480.00002) * 480;
					float GridLocationZ = FMath::RoundHalfFromZero(ProjectedLocation.Z / 480.00002) * 480;
					FVector GridLocation = FVector(GridLocationX, GridLocationY, GridLocationZ);
					FRotator ProjectedRotation = GetActorRotation().Add(0, 90, 0); // projected rotation before setting to grid using the actor's rotation
					float GridRotationYaw = FMath::RoundHalfFromZero(ProjectedRotation.Yaw / 90.0) * 90;

					FRotator GridRotation = FRotator(0, GridRotationYaw, 0);
					/*FString LogMsg = FString("GridLocation X ") + FString::SanitizeFloat(GridLocation.X) + FString(" ProjectedLocation X") + FString::SanitizeFloat(ProjectedLocation.X);
					UE_LOG(LogMyGame, Warning, TEXT("%s"), *LogMsg);*/
					// to make the structures connect with each other, have to add an offset when in a different rotation
					if (FMath::Abs(GridRotationYaw) == 0) {
						GridLocation.Y -= 240.00001;
						if (ProjectedLocation.X < GridLocation.X) {
							GridLocation.X -= 240.00001;
						}
						else {
							GridLocation.X += 240.00001;
						}
					}
					if (FMath::Abs(GridRotationYaw) == 180) {
						GridLocation.Y += 240.00001;
						if (ProjectedLocation.X > GridLocation.X) {
							GridLocation.X += 240.00001;
						}
						else {
							GridLocation.X -= 240.00001;
						}
					}

					ABuildingActor* Wall = GetWorld()->SpawnActor<ABuildingActor>(WallClasses[CurrentBuildingMaterial], GridLocation, GridRotation);

					Wall->GetOverlappingActors(OverlappingActors);

					for (int i = 0; i < OverlappingActors.Num(); i++) {
						if (OverlappingActors[i] != nullptr) {
							if (OverlappingActors[i]->IsA(AFortniteCloneCharacter::StaticClass())) {
								//don't allow a player to build a structure that overlaps with another player
								Wall->Destroy();
								return;
							}
							else if (OverlappingActors[i]->IsA(ABuildingActor::StaticClass())) {
								ABuildingActor* Structure = Cast<ABuildingActor>(OverlappingActors[i]);
								//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Overlapped with another building?") + Structure->StructureType);
								if (!Structure->IsPreview && Structure->StructureType.Equals(Wall->StructureType) && FMath::Abs(FVector::Distance(Structure->GetTransform().GetLocation(), Wall->GetTransform().GetLocation())) < 1.f) {
									//don't allow a player to build a structure in the exact same spot that another structure of its same kind was already built
									//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Overlapped with another building?") + FString::SanitizeFloat(FMath::Abs(FVector::Distance(Structure->GetTransform().GetLocation(), Wall->GetTransform().GetLocation()))));
									Wall->Destroy();
									return;
								}
							}
						}
					}
					Wall->Id = CurrentStructureId;
					CurrentStructureId += 1;
					ClientSpawnedStructures.Add(Wall);

					State->MaterialCounts[CurrentBuildingMaterial] -= 10;

					ServerBuildStructure(WallClasses[CurrentBuildingMaterial], GridLocation, GridRotation, Wall->Id);
					ServerSetMaterialCount(State->MaterialCounts[CurrentBuildingMaterial], CurrentBuildingMaterial);
				}
				else if (State->InBuildMode && State->BuildMode == FString("Ramp") && State->MaterialCounts[CurrentBuildingMaterial] >= 10) {
					TArray<AActor*> OverlappingActors;
					//original 400, 200, 2
					FVector ProjectedLocation = GetActorLocation() + (GetActorForwardVector() * FVector(50.0, 6.25, 1.0)) + (FVector(0, 0, DirectionVector.Z) * 1.5); // projected location before setting to grid using actor forward and direction (aim offset) vectors
					float GridLocationX = FMath::RoundHalfFromZero(ProjectedLocation.X / 480.00002) * 480;
					float GridLocationY = FMath::RoundHalfFromZero(ProjectedLocation.Y / 480.00002) * 480;
					float GridLocationZ = FMath::RoundHalfFromZero(ProjectedLocation.Z / 480.00002) * 480;
					FVector GridLocation = FVector(GridLocationX, GridLocationY, GridLocationZ);
					FRotator ProjectedRotation = GetActorRotation().Add(0, 90, 0); // projected rotation before setting to grid using the actor's rotation
					float GridRotationYaw = FMath::RoundHalfFromZero(ProjectedRotation.Yaw / 90.0) * 90;

					FRotator GridRotation = FRotator(0, GridRotationYaw, 0);

					if (FMath::Abs(GridRotationYaw) == 0) {
						GridLocation.Y -= 240.00001;
						if (ProjectedLocation.X < GridLocation.X) {
							GridLocation.X -= 240.00001;
						}
						else {
							GridLocation.X += 240.00001;
						}
					}
					if (FMath::Abs(GridRotationYaw) == 180) {
						GridLocation.Y += 240.00001;
						if (ProjectedLocation.X > GridLocation.X) {
							GridLocation.X += 240.00001;
						}
						else {
							GridLocation.X -= 240.00001;
						}
					}

					ABuildingActor* Ramp = GetWorld()->SpawnActor<ABuildingActor>(RampClasses[CurrentBuildingMaterial], GridLocation, GridRotation);

					Ramp->GetOverlappingActors(OverlappingActors);

					for (int i = 0; i < OverlappingActors.Num(); i++) {
						if (OverlappingActors[i] != nullptr) {
							if (OverlappingActors[i]->IsA(AFortniteCloneCharacter::StaticClass())) {
								//don't allow a player to build a structure that overlaps with another player
								Ramp->Destroy();
								return;
							}
							else if (OverlappingActors[i]->IsA(ABuildingActor::StaticClass())) {
								ABuildingActor* Structure = Cast<ABuildingActor>(OverlappingActors[i]);
								if (!Structure->IsPreview && Structure->StructureType.Equals(Ramp->StructureType) && FMath::Abs(FVector::Distance(Structure->GetTransform().GetLocation(), Ramp->GetTransform().GetLocation())) < 1.f) {
									//don't allow a player to build a structure in the exact same spot that another structure of its same kind was already built
									Ramp->Destroy();
									return;
								}
							}
						}
					}
					Ramp->Id = CurrentStructureId;
					CurrentStructureId += 1;
					ClientSpawnedStructures.Add(Ramp);

					State->MaterialCounts[CurrentBuildingMaterial] -= 10;

					ServerBuildStructure(RampClasses[CurrentBuildingMaterial], GridLocation, GridRotation, Ramp->Id);
					ServerSetMaterialCount(State->MaterialCounts[CurrentBuildingMaterial], CurrentBuildingMaterial);
				}
				else if (State->InBuildMode && State->BuildMode == FString("Floor") && State->MaterialCounts[CurrentBuildingMaterial] >= 10) {
					TArray<AActor*> OverlappingActors;

					FVector ProjectedLocation = GetActorLocation() + (GetActorForwardVector() * FVector(100.0, 25.0, 1.0)) + (FVector(0, 0, DirectionVector.Z) * 2.0); // projected location before setting to grid using actor forward and direction (aim offset) vectors
					float GridLocationX = FMath::RoundHalfFromZero(ProjectedLocation.X / 480.00002) * 480;
					float GridLocationY = FMath::RoundHalfFromZero(ProjectedLocation.Y / 480.00002) * 480;
					float GridLocationZ = FMath::RoundHalfFromZero(ProjectedLocation.Z / 480.00002) * 480;
					FVector GridLocation = FVector(GridLocationX, GridLocationY, GridLocationZ);
					FRotator ProjectedRotation = GetActorRotation().Add(0, 90, 0); // projected rotation before setting to grid using the actor's rotation
					float GridRotationYaw = FMath::RoundHalfFromZero(ProjectedRotation.Yaw / 90.0) * 90;

					FRotator GridRotation = FRotator(0, GridRotationYaw, 0);

					if (FMath::Abs(GridRotationYaw) == 0) {
						GridLocation.Y -= 240.00001;
						if (ProjectedLocation.X < GridLocation.X) {
							GridLocation.X -= 240.00001;
						}
						else {
							GridLocation.X += 240.00001;
						}
					}
					if (FMath::Abs(GridRotationYaw) == 180) {
						GridLocation.Y += 240.00001;
						if (ProjectedLocation.X > GridLocation.X) {
							GridLocation.X += 240.00001;
						}
						else {
							GridLocation.X -= 240.00001;
						}
					}

					ABuildingActor* Floor = GetWorld()->SpawnActor<ABuildingActor>(FloorClasses[CurrentBuildingMaterial], GridLocation, GridRotation);

					Floor->GetOverlappingActors(OverlappingActors);

					for (int i = 0; i < OverlappingActors.Num(); i++) {
						if (OverlappingActors[i] != nullptr) {
							if (OverlappingActors[i]->IsA(AFortniteCloneCharacter::StaticClass())) {
								//don't allow a player to build a structure that overlaps with another player
								Floor->Destroy();
								return;
							}
							else if (OverlappingActors[i]->IsA(ABuildingActor::StaticClass())) {
								ABuildingActor* Structure = Cast<ABuildingActor>(OverlappingActors[i]);
								if (!Structure->IsPreview && Structure->StructureType.Equals(Floor->StructureType) && FMath::Abs(FVector::Distance(Structure->GetTransform().GetLocation(), Floor->GetTransform().GetLocation())) < 1.f) {
									//don't allow a player to build a structure in the exact same spot that another structure of its same kind was already built
									//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Overlapped with another building?") + FString::SanitizeFloat(FMath::Abs(FVector::Distance(Structure->GetTransform().GetLocation(), Floor->GetTransform().GetLocation()))));

									Floor->Destroy();
									return;
								}
							}
						}
					}
					Floor->Id = CurrentStructureId;
					CurrentStructureId += 1;
					ClientSpawnedStructures.Add(Floor);

					State->MaterialCounts[CurrentBuildingMaterial] -= 10;

					ServerBuildStructure(FloorClasses[CurrentBuildingMaterial], GridLocation, GridRotation, Floor->Id);
					ServerSetMaterialCount(State->MaterialCounts[CurrentBuildingMaterial], CurrentBuildingMaterial);
				}
			}
		}
	}
}

void AFortniteCloneCharacter::SwitchBuildingMaterial() {
	ServerChangeBuildingMaterial();
}

void AFortniteCloneCharacter::ShootGun() {
	ServerFireWeapon();
}

void AFortniteCloneCharacter::UseHealingItem() {
	ServerUseHealingItem(CurrentHealingItemType);
}

void AFortniteCloneCharacter::Reload() {
	ServerReloadWeapons();
}

void AFortniteCloneCharacter::AimGunIn() {
	ServerAimDownSights();
}

void AFortniteCloneCharacter::AimGunOut() {
	ServerAimHipFire();
}

void AFortniteCloneCharacter::PickaxeTimeOut() {
	ServerPickaxeTimeOut();
}

void AFortniteCloneCharacter::RifleTimeOut() {
	ServerRifleTimeOut();
}

void AFortniteCloneCharacter::ShotgunTimeOut() {
	ServerShotgunTimeOut();
}

void AFortniteCloneCharacter::RifleReloadTimeOut() {
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	State->JustReloadedRifle= false;
}

void AFortniteCloneCharacter::ShotgunReloadTimeOut() {
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	State->JustReloadedShotgun = false;
}

void AFortniteCloneCharacter::HoldPickaxe() {
	ServerSwitchToPickaxe();
}

void AFortniteCloneCharacter::HoldAssaultRifle() {
	ServerSwitchToRifle();
}

void AFortniteCloneCharacter::HoldShotgun() {
	ServerSwitchToShotgun();
}

void AFortniteCloneCharacter::HoldBandage() {
	ServerSwitchToHealingItem(0);
}

void AFortniteCloneCharacter::HoldPotion() {
	ServerSwitchToHealingItem(1);
}

float AFortniteCloneCharacter::GetHealth() {
	return Health;
}

float AFortniteCloneCharacter::GetShield() {
	return Shield;
}

int AFortniteCloneCharacter::GetWoodMaterialCount() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			return State->MaterialCounts[0];
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

int AFortniteCloneCharacter::GetStoneMaterialCount() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			return State->MaterialCounts[1];
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

int AFortniteCloneCharacter::GetSteelMaterialCount() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			return State->MaterialCounts[2];
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

int AFortniteCloneCharacter::GetAssaultRifleAmmoCount() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			return State->EquippedWeaponsAmmunition[1] + State->EquippedWeaponsClips[1];
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

int AFortniteCloneCharacter::GetShotgunAmmoCount() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			return State->EquippedWeaponsAmmunition[2] + State->EquippedWeaponsClips[2];
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

int AFortniteCloneCharacter::GetHealingItemCount(int HealingItemType) {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			return State->HealingItemCounts[HealingItemType];
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

void AFortniteCloneCharacter::OpenSettingsMenu() {
	if (GetController()) {
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController->GetHUD()) {
			AFortniteCloneHUD* FortniteCloneHUD = Cast<AFortniteCloneHUD>(PlayerController->GetHUD());
			FortniteCloneHUD->DrawSettingsMenu();
		}
	}
}

void AFortniteCloneCharacter::ServerSetIsWalkingTrue_Implementation() {
	IsWalking = true;
}

bool AFortniteCloneCharacter::ServerSetIsWalkingTrue_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerSetIsWalkingFalse_Implementation() {
	IsWalking = false;
}

bool AFortniteCloneCharacter::ServerSetIsWalkingFalse_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerSetIsRunningTrue_Implementation() {
	IsRunning = true;
}

bool AFortniteCloneCharacter::ServerSetIsRunningTrue_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerSetIsRunningFalse_Implementation() {
	IsRunning = false;
}

bool AFortniteCloneCharacter::ServerSetIsRunningFalse_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerSetWalkingSpeed_Implementation() {
	GetCharacterMovement()->MaxWalkSpeed = 450.0;
	ClientSetWalkingSpeed();
}

bool AFortniteCloneCharacter::ServerSetWalkingSpeed_Validate() {
	return true;
}

void AFortniteCloneCharacter::ClientSetWalkingSpeed_Implementation() {
	GetCharacterMovement()->MaxWalkSpeed = 450.0;
}

void AFortniteCloneCharacter::ServerSetRunningSpeed_Implementation() {
	GetCharacterMovement()->MaxWalkSpeed = 900.0;
	ClientSetRunningSpeed();
}

bool AFortniteCloneCharacter::ServerSetRunningSpeed_Validate() {
	return true;
}

void AFortniteCloneCharacter::ClientSetRunningSpeed_Implementation() {
	GetCharacterMovement()->MaxWalkSpeed = 900.0;
}

void AFortniteCloneCharacter::ServerSetAimedInSpeed_Implementation() {
	GetCharacterMovement()->MaxWalkSpeed = 200.0;
	ClientSetAimedInSpeed();
}

bool AFortniteCloneCharacter::ServerSetAimedInSpeed_Validate() {
	return true;
}

void AFortniteCloneCharacter::ClientSetAimedInSpeed_Implementation() {
	GetCharacterMovement()->MaxWalkSpeed = 200.0;
}

void AFortniteCloneCharacter::ServerSetMovingForwards_Implementation() {
	WalkingY = 90;
	RunningY = 90;
}

bool AFortniteCloneCharacter::ServerSetMovingForwards_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerSetMovingBackwards_Implementation() {
	WalkingY = -90;
	RunningY = -90;
}

bool AFortniteCloneCharacter::ServerSetMovingBackwards_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerSetMovingLeft_Implementation() {
	WalkingX = -90;
	RunningX = -90;
}

bool AFortniteCloneCharacter::ServerSetMovingLeft_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerSetMovingRight_Implementation() {
	WalkingX = 90;
	RunningX = 90;
}

bool AFortniteCloneCharacter::ServerSetMovingRight_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerResetMovingForward_Implementation() {
	WalkingY = 0;
	RunningY = 0;
}

bool AFortniteCloneCharacter::ServerResetMovingForward_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerResetMovingRight_Implementation() {
	WalkingX = 0;
	RunningX = 0;
}

bool AFortniteCloneCharacter::ServerResetMovingRight_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerSetMaterialCount_Implementation(int Count, int MaterialType) {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			State->MaterialCounts[MaterialType] = Count;
		}
	}
}

bool AFortniteCloneCharacter::ServerSetMaterialCount_Validate(int Count, int MaterialType) {
	return true;
}

void AFortniteCloneCharacter::ServerBuildStructure_Implementation(TSubclassOf<ABuildingActor> StructureClass, FVector SpawnLocation, FRotator SpawnRotation, int StructureId) {
	if (StructureClass != nullptr) {
		GetWorld()->SpawnActor<ABuildingActor>(StructureClass, SpawnLocation, SpawnRotation);

		ClientDestroyStructure(StructureId);
	}
}

bool AFortniteCloneCharacter::ServerBuildStructure_Validate(TSubclassOf<ABuildingActor> StructureClass, FVector SpawnLocation, FRotator SpawnRotation, int StructureId) {
	return true;
}

void AFortniteCloneCharacter::ServerSetBuildModeWall_Implementation() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			if (State->JustUsedHealingItem || State->JustReloadedRifle || State->JustReloadedShotgun) {
				return; //currently healing or reloading
			}
			if (State->HoldingWeapon && State->AimedIn) {
				return; // currently aimed down sight
			}
			if (State->BuildMode == FString("Wall")) {
				// getting out of build mode
				State->InBuildMode = false;
				State->BuildMode = FString("None");
				if (BuildingPreview) {
					BuildingPreview->Destroy(); //destroy the last wall preview
				}
				// equip weapon being held before
				if (CurrentWeaponType > -1 && CurrentWeaponType < 3) {
					ClientGetWeaponTransform(CurrentWeaponType);
				}
				else if (CurrentHealingItemType > -1 && CurrentHealingItemType < 2) {
					//equip healing item since current weapon was null
					ClientGetHealingItemTransform(CurrentHealingItemType);
				}
			}
			else if (State->InBuildMode) {
				// switching to a different build mode
				State->BuildMode = FString("Wall");
			}
			else {
				// getting into build mode
				State->InBuildMode = true;
				State->BuildMode = FString("Wall");
				State->HoldingWeapon = false;
				State->HoldingHealingItem = false;
				State->AimedIn = false; 
				//animinstance properties
				HoldingWeapon = false;
				AimedIn = false;
				HoldingWeaponType = 0;

				// unequip weapon/healing item
				if (CurrentWeapon && CurrentWeaponType > 0 && CurrentWeaponType < 3) {
					State->EquippedWeaponsClips[CurrentWeaponType] = CurrentWeapon->CurrentBulletCount;
				}
				if (CurrentWeapon) {
					CurrentWeapon->Destroy();
					CurrentWeapon = nullptr;
				}
				if (CurrentHealingItem) {
					CurrentHealingItem->Destroy();
					CurrentHealingItem = nullptr;
				}
			}
		}
	}
}

bool AFortniteCloneCharacter::ServerSetBuildModeWall_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerSetBuildModeRamp_Implementation() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			if (State->JustUsedHealingItem || State->JustReloadedRifle || State->JustReloadedShotgun) {
				return; //currently healing or reloading
			}
			if (State->HoldingWeapon && State->AimedIn) {
				return; // currently aimed down sight
			}
			if (State->BuildMode == FString("Ramp")) {
				// getting out of build mode
				State->InBuildMode = false;
				State->BuildMode = FString("None");
				if (BuildingPreview) {
					BuildingPreview->Destroy(); //destroy the last wall preview
				}
				// equip weapon being held before
				if (CurrentWeaponType > -1 && CurrentWeaponType < 3) {
					ClientGetWeaponTransform(CurrentWeaponType);
				}
				else if(CurrentHealingItemType > -1 && CurrentHealingItemType < 2){
					//equip healing item since current weapon was null
					ClientGetHealingItemTransform(CurrentHealingItemType);
				}
			}
			else if (State->InBuildMode) {
				// switching to a different build mode
				State->BuildMode = FString("Ramp");
			}
			else {
				// getting into build mode
				State->InBuildMode = true;
				State->BuildMode = FString("Ramp");
				State->HoldingWeapon = false;
				State->HoldingHealingItem = false;
				State->AimedIn = false;
				//animinstance properties
				HoldingWeapon = false;
				AimedIn = false;
				HoldingWeaponType = 0;
				// unequip weapon/healing item
				if (CurrentWeapon && CurrentWeaponType > 0 && CurrentWeaponType < 3) {
					State->EquippedWeaponsClips[CurrentWeaponType] = CurrentWeapon->CurrentBulletCount;
				}
				if (CurrentWeapon) {
					CurrentWeapon->Destroy();
					CurrentWeapon = nullptr;
				}
				if (CurrentHealingItem) {
					CurrentHealingItem->Destroy();
					CurrentHealingItem = nullptr;
				}
			}
		}
	}
}

bool AFortniteCloneCharacter::ServerSetBuildModeRamp_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerSetBuildModeFloor_Implementation() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			if (State->JustUsedHealingItem || State->JustReloadedRifle || State->JustReloadedShotgun || State->JustSwungPickaxe) {
				return; //currently healing or reloading or swinging pickaxe
			}
			if (State->HoldingWeapon && State->AimedIn) {
				return; // currently aimed down sight
			}
			if (State->BuildMode == FString("Floor")) {
				// getting out of build mode
				State->InBuildMode = false;
				State->BuildMode = FString("None");
				if (BuildingPreview) {
					BuildingPreview->Destroy(); //destroy the last wall preview
				}
				// equip weapon being held before
				if (CurrentWeaponType > -1 && CurrentWeaponType < 3) {
					ClientGetWeaponTransform(CurrentWeaponType);
				}
				else if (CurrentHealingItemType > -1 && CurrentHealingItemType < 2) {
					//equip healing item since current weapon was null
					ClientGetHealingItemTransform(CurrentHealingItemType);
				}
			}
			else if (State->InBuildMode) {
				// switching to a different build mode
				State->BuildMode = FString("Floor");
			}
			else {
				// getting into build mode
				State->InBuildMode = true;
				State->BuildMode = FString("Floor");
				State->HoldingWeapon = false;
				State->HoldingHealingItem = false;
				State->AimedIn = false;
				//animinstance properties
				HoldingWeapon = false;
				AimedIn = false;
				HoldingWeaponType = 0;
				// unequip weapon/healing item
				if (CurrentWeapon && CurrentWeaponType > 0 && CurrentWeaponType < 3) {
					State->EquippedWeaponsClips[CurrentWeaponType] = CurrentWeapon->CurrentBulletCount;
				}
				if (CurrentWeapon) {
					CurrentWeapon->Destroy();
					CurrentWeapon = nullptr;
				}
				if (CurrentHealingItem) {
					CurrentHealingItem->Destroy();
					CurrentHealingItem = nullptr;
				}
			}
		}
	}
}

bool AFortniteCloneCharacter::ServerSetBuildModeFloor_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerFireWeapon_Implementation() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(GetNetMode()) + FString(" Current weapon ") + FString::FromInt(State->CurrentWeapon));
			if (State->HoldingWeapon) {
				if (State->CurrentWeapon > 0 && State->CurrentWeapon < 3 && CurrentWeapon->CurrentBulletCount <= 0) {
					// no bullets in magazine, need to reload
					ServerReloadWeapons();
					return;
				}
				if (State->JustReloadedRifle || State->JustReloadedShotgun) {
					return; //currently reloading
				}
				if (State->AimedIn) {
					if (State->CurrentWeapon == 1) {
						if (State->JustShotRifle) {
							return;
						}
						NetMulticastPlayShootRifleIronsightsAnimation();
						CurrentWeapon->CurrentBulletCount--;
						State->EquippedWeaponsClips[CurrentWeaponType]--;
						State->JustShotRifle = true;
						FTimerHandle RifleTimerHandle;
						GetWorldTimerManager().SetTimer(RifleTimerHandle, this, &AFortniteCloneCharacter::ServerRifleTimeOut, 0.233f, false);
					}
					else if (State->CurrentWeapon == 2) {
						if (State->JustShotShotgun) {
							return;
						}
						NetMulticastPlayShootShotgunIronsightsAnimation();
						CurrentWeapon->CurrentBulletCount--;
						State->EquippedWeaponsClips[CurrentWeaponType]--;
						State->JustShotShotgun = true;
						FTimerHandle ShotgunTimerHandle;
						GetWorldTimerManager().SetTimer(ShotgunTimerHandle, this, &AFortniteCloneCharacter::ServerShotgunTimeOut, 1.3f, false);
					}
				}
				else {
					if (State->CurrentWeapon == 0) {
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "pickaxe swung");
						if (State->JustSwungPickaxe) {
							return;
						}
						NetMulticastPlayPickaxeSwingAnimation();
						State->JustSwungPickaxe = true;
						FTimerHandle PickaxeTimerHandle;
						GetWorldTimerManager().SetTimer(PickaxeTimerHandle, this, &AFortniteCloneCharacter::ServerPickaxeTimeOut, 0.403f, false);
					}
					if (State->CurrentWeapon == 1) {
						if (State->JustShotRifle) {
							return;
						}
						NetMulticastPlayShootRifleAnimation();
						CurrentWeapon->CurrentBulletCount--;
						State->EquippedWeaponsClips[CurrentWeaponType]--;
						State->JustShotRifle = true;
						FTimerHandle RifleTimerHandle;
						GetWorldTimerManager().SetTimer(RifleTimerHandle, this, &AFortniteCloneCharacter::ServerRifleTimeOut, 0.233f, false);
					}
					else if (State->CurrentWeapon == 2) {
						if (State->JustShotShotgun) {
							return;
						}
						NetMulticastPlayShootShotgunAnimation();
						CurrentWeapon->CurrentBulletCount--;
						State->EquippedWeaponsClips[CurrentWeaponType]--;
						State->JustShotShotgun = true;
						FTimerHandle ShotgunTimerHandle;
						GetWorldTimerManager().SetTimer(ShotgunTimerHandle, this, &AFortniteCloneCharacter::ServerShotgunTimeOut, 1.3f, false);
					}

				}
				ClientGetBulletTransform();
				

			}
		}
	}
}

bool AFortniteCloneCharacter::ServerFireWeapon_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerUseHealingItem_Implementation(int HealingItemType) {
	if (HealingItemType == 0) {
		if (Health >= 100) {
			return; // player has full health
		}
	}
	else if (HealingItemType == 1) {
		if (Shield >= 100) {
			return; // player has full shield
		}
	}
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State && State->HoldingHealingItem) {
			if (State->HealingItemCounts[HealingItemType] < 1) {
				return; // player has no healing items to use
			}

			if (State->JustUsedHealingItem) {
				return;
			}
			NetMulticastPlayUseHealingItemAnimation();
			State->JustUsedHealingItem = true;
			State->HealingItemCounts[HealingItemType]--;
			FTimerHandle HealingItemTimerHandle;
			FTimerDelegate HealingItemTimerDelegate;
			HealingItemTimerDelegate.BindUFunction(this, FName("ServerHealingItemTimeOut"), HealingItemType);
			GetWorldTimerManager().SetTimer(HealingItemTimerHandle, HealingItemTimerDelegate, 3.321f, false);
		}
	}
}

bool AFortniteCloneCharacter::ServerUseHealingItem_Validate(int HealingItemType) {
	return true;
}


void AFortniteCloneCharacter::ServerReloadWeapons_Implementation() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			if (State->CurrentWeapon > 2 || State->CurrentWeapon < 1) {
				return; // can only reload if holding a assault rifle or shotgun
			}
			if (State->JustShotRifle || State->JustShotShotgun || State->JustReloadedRifle || State->JustReloadedShotgun) {
				return; // currently reloading or just shot
			}
			if (State->AimedIn) {
				if (State->CurrentWeapon == 1) {
					if (State->JustShotRifle) {
						return;
					}
					if (State->EquippedWeaponsAmmunition[State->CurrentWeapon] <= 0) {
						return; // no ammo left
					}

					int BulletsNeeded = CurrentWeapon->MagazineSize - CurrentWeapon->CurrentBulletCount;
					if (BulletsNeeded == 0) {
						return; // magazine is full
					}

					if (State->EquippedWeaponsAmmunition[State->CurrentWeapon] < BulletsNeeded) {
						BulletsNeeded = State->EquippedWeaponsAmmunition[State->CurrentWeapon];
						State->EquippedWeaponsAmmunition[State->CurrentWeapon] = 0;
					}
					else {
						State->EquippedWeaponsAmmunition[State->CurrentWeapon] -= BulletsNeeded;
					}
					NetMulticastPlayReloadRifleIronsightsAnimation();
					CurrentWeapon->CurrentBulletCount += BulletsNeeded;
					State->EquippedWeaponsClips[State->CurrentWeapon] += BulletsNeeded;
					State->JustReloadedRifle = true;
					FTimerHandle RifleTimerHandle;
					GetWorldTimerManager().SetTimer(RifleTimerHandle, this, &AFortniteCloneCharacter::ServerRifleReloadTimeOut, 2.167f, false);
				}
				else if (State->CurrentWeapon == 2) {
					if (State->JustShotShotgun) {
						return;
					}
					if (State->EquippedWeaponsAmmunition[State->CurrentWeapon] <= 0) {
						return; // no ammo left
					}

					int BulletsNeeded = CurrentWeapon->MagazineSize - CurrentWeapon->CurrentBulletCount;
					if (BulletsNeeded == 0) {
						return; // magazine is full
					}

					if (State->EquippedWeaponsAmmunition[State->CurrentWeapon] < BulletsNeeded) {
						BulletsNeeded = State->EquippedWeaponsAmmunition[State->CurrentWeapon];
						State->EquippedWeaponsAmmunition[State->CurrentWeapon] = 0;
					}
					else {
						State->EquippedWeaponsAmmunition[State->CurrentWeapon] -= BulletsNeeded;
					}
					NetMulticastPlayReloadShotgunIronsightsAnimation();
					CurrentWeapon->CurrentBulletCount += BulletsNeeded;
					State->EquippedWeaponsClips[State->CurrentWeapon] += BulletsNeeded;
					State->JustReloadedShotgun = true;
					FTimerHandle ShotgunTimerHandle;
					GetWorldTimerManager().SetTimer(ShotgunTimerHandle, this, &AFortniteCloneCharacter::ServerShotgunReloadTimeOut, 4.3f, false);
				}
			}
			else {
				if (State->CurrentWeapon == 1) {
					if (State->JustShotRifle) {
						return;
					}
					if (State->EquippedWeaponsAmmunition[State->CurrentWeapon] <= 0) {
						return; // no ammo left
					}
					int BulletsNeeded = CurrentWeapon->MagazineSize - CurrentWeapon->CurrentBulletCount;
					if (BulletsNeeded == 0) {
						return; // magazine is full
					}
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(State->EquippedWeaponsAmmunition[State->CurrentWeapon]));
					if (State->EquippedWeaponsAmmunition[State->CurrentWeapon] < BulletsNeeded) {
						BulletsNeeded = State->EquippedWeaponsAmmunition[State->CurrentWeapon];
						State->EquippedWeaponsAmmunition[State->CurrentWeapon] = 0;
					}
					else {
						State->EquippedWeaponsAmmunition[State->CurrentWeapon] -= BulletsNeeded;
					}
					NetMulticastPlayReloadRifleAnimation();
					CurrentWeapon->CurrentBulletCount += BulletsNeeded;
					State->EquippedWeaponsClips[State->CurrentWeapon] += BulletsNeeded;
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(CurrentWeapon->CurrentBulletCount));
					State->JustReloadedRifle = true;
					FTimerHandle RifleTimerHandle;
					GetWorldTimerManager().SetTimer(RifleTimerHandle, this, &AFortniteCloneCharacter::ServerRifleReloadTimeOut, 2.167f, false);
				}
				else if (State->CurrentWeapon == 2) {
					if (State->JustShotShotgun) {
						return;
					}
					if (State->EquippedWeaponsAmmunition[State->CurrentWeapon] <= 0) {
						return; // no ammo left
					}
					int BulletsNeeded = CurrentWeapon->MagazineSize - CurrentWeapon->CurrentBulletCount;
					if (BulletsNeeded == 0) {
						return; // magazine is full
					}

					if (State->EquippedWeaponsAmmunition[State->CurrentWeapon] < BulletsNeeded) {
						BulletsNeeded = State->EquippedWeaponsAmmunition[State->CurrentWeapon];
						State->EquippedWeaponsAmmunition[State->CurrentWeapon] = 0;
					}
					else {
						State->EquippedWeaponsAmmunition[State->CurrentWeapon] -= BulletsNeeded;
					}
					NetMulticastPlayReloadShotgunAnimation();
					CurrentWeapon->CurrentBulletCount += BulletsNeeded;
					State->EquippedWeaponsClips[State->CurrentWeapon] += BulletsNeeded;
					State->JustReloadedShotgun = true;
					FTimerHandle ShotgunTimerHandle;
					GetWorldTimerManager().SetTimer(ShotgunTimerHandle, this, &AFortniteCloneCharacter::ServerShotgunReloadTimeOut, 4.3f, false);
				}

			}
		}
	}
}

bool AFortniteCloneCharacter::ServerReloadWeapons_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerSwitchToPickaxe_Implementation() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			if (State->CurrentWeapon == 0 && !State->InBuildMode) {
				return; // currently holding a pickaxe while not in build mode
			}
			if (State->HoldingWeapon && State->AimedIn) {
				return; // currently aimed down sight
			}
			if (State->JustUsedHealingItem || State->JustReloadedRifle || State->JustReloadedShotgun || State->JustShotRifle || State->JustShotShotgun) {
				return; // currently healing or currently reloading or just shot a weapon
			}
			else {
				if (State->InBuildMode) {
					State->InBuildMode = false;
					State->BuildMode = FString("None");
					if (BuildingPreview) {
						BuildingPreview->Destroy(); //destroy the last wall preview
					}
				}
				if (CurrentWeapon) {
					State->EquippedWeaponsClips[CurrentWeaponType] = CurrentWeapon->CurrentBulletCount;
					CurrentWeapon->Destroy();
					CurrentWeapon = nullptr;
				}
				if (CurrentHealingItem) {
					CurrentHealingItem->Destroy();
					CurrentHealingItem = nullptr;
				}
				ClientGetWeaponTransform(0);
			}
		}
	}
}

bool AFortniteCloneCharacter::ServerSwitchToPickaxe_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerSwitchToRifle_Implementation() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			if (State->CurrentWeapon == 1 && !State->InBuildMode) {
				return; // currently holding a assault rifle while not in build mode
			}
			if (State->HoldingWeapon && State->AimedIn) {
				return; // currently aimed down sight
			}
			if (!State->EquippedWeapons.Contains(1) || State->JustUsedHealingItem || State->JustReloadedRifle || State->JustReloadedShotgun || State->JustSwungPickaxe || State->JustShotShotgun) {
				return; // already holding the assault rifle or doesn't have one or is currently healing or currently reloading or swinging pickaxe or shooting shotgun
			}
			else {
				if (State->InBuildMode) {
					State->InBuildMode = false;
					State->BuildMode = FString("None");
					if (BuildingPreview) {
						BuildingPreview->Destroy(); //destroy the last wall preview
					}
				}
				if (CurrentWeapon && CurrentWeaponType > 0 && CurrentWeaponType < 3) {
					State->EquippedWeaponsClips[CurrentWeaponType] = CurrentWeapon->CurrentBulletCount;
				}
				if (CurrentWeapon) {
					CurrentWeapon->Destroy();
					CurrentWeapon = nullptr;
				}
				if (CurrentHealingItem) {
					CurrentHealingItem->Destroy();
					CurrentHealingItem = nullptr;
				}
				ClientGetWeaponTransform(1);
			}
		}
	}
}

bool AFortniteCloneCharacter::ServerSwitchToRifle_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerSwitchToShotgun_Implementation() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			if (State->CurrentWeapon == 2 && !State->InBuildMode) {
				return; // currently holding a shotgun while not in build mode
			}
			if (State->HoldingWeapon && State->AimedIn) {
				return; // currently aimed down sight
			}
			if (!State->EquippedWeapons.Contains(2) || State->JustUsedHealingItem || State->JustReloadedRifle || State->JustReloadedShotgun || State->JustSwungPickaxe || State->JustShotRifle) {
				return; // already holding the pickaxe or doesn't have one or is currently healing or currently reloading or swinging pickaxe or just shot rifle
			}
			else {
				if (State->InBuildMode) {
					State->InBuildMode = false;
					State->BuildMode = FString("None");
					if (BuildingPreview) {
						BuildingPreview->Destroy(); //destroy the last wall preview
					}
				}
				if (CurrentWeapon && CurrentWeaponType > 0 && CurrentWeaponType < 3) {
					State->EquippedWeaponsClips[CurrentWeaponType] = CurrentWeapon->CurrentBulletCount;
				}
				if (CurrentWeapon) {
					CurrentWeapon->Destroy();
					CurrentWeapon = nullptr;
				}
				if (CurrentHealingItem) {
					CurrentHealingItem->Destroy();
					CurrentHealingItem = nullptr;
				}
				ClientGetWeaponTransform(2);
			}
		}
	}
}

bool AFortniteCloneCharacter::ServerSwitchToShotgun_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerSwitchToHealingItem_Implementation(int HealingItemType) {
	/*FString LogMsg = FString("server switch to healing item Current healing item type ") + FString::FromInt(HealingItemType);
	UE_LOG(LogMyGame, Warning, TEXT("%s"), *LogMsg);*/
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			if (State->JustReloadedRifle || State->JustReloadedShotgun || State->JustSwungPickaxe || State->JustShotRifle || State->JustShotShotgun) {
				return; //currently reloading weapons or swinging pickaxe
			}
			if (State->HoldingWeapon && State->AimedIn) {
				return; // currently aimed down sight
			}
			if (State->CurrentHealingItem == HealingItemType && !State->InBuildMode) {
				return; // already holding healing items while not in build mode
			}
			else {
				if (State->InBuildMode) {
					State->InBuildMode = false;
					State->BuildMode = FString("None");
					if (BuildingPreview) {
						BuildingPreview->Destroy(); //destroy the last wall preview
					}
				}
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(CurrentWeaponType));
				if (CurrentWeapon && CurrentWeaponType > 0 && CurrentWeaponType < 3) {
					State->EquippedWeaponsClips[CurrentWeaponType] = CurrentWeapon->CurrentBulletCount;
				}
				if (CurrentWeapon) {
					CurrentWeapon->Destroy();
					CurrentWeapon = nullptr;
				}
				if (CurrentHealingItem) {
					CurrentHealingItem->Destroy();
					CurrentHealingItem = nullptr;
				}
				ClientGetHealingItemTransform(HealingItemType);
			}
		}
	}
}

bool AFortniteCloneCharacter::ServerSwitchToHealingItem_Validate(int HealingItemType) {
	return true;
}

void AFortniteCloneCharacter::ServerChangeBuildingMaterial_Implementation() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State && State->InBuildMode) {
			if (CurrentBuildingMaterial == 2) {
				CurrentBuildingMaterial = 0;
			}
			else {
				CurrentBuildingMaterial++;
			}
		}
	}
}

bool AFortniteCloneCharacter::ServerChangeBuildingMaterial_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerAimDownSights_Implementation() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State && State->HoldingWeapon && State->CurrentWeapon != 0) {
			AimedIn = true;
			HoldingWeaponType = 2;
			ClientCameraAimIn();
			//ServerSetAimedInSpeed();
			State->AimedIn = true;
		}
	}
}

bool AFortniteCloneCharacter::ServerAimDownSights_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerAimHipFire_Implementation() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State && State->HoldingWeapon && State->CurrentWeapon != 0) {
			AimedIn = false;
			HoldingWeaponType = 1;
			ClientCameraAimOut();
			//ServerSetWalkingSpeed();
			State->AimedIn = false;
		}
	}
	
}

bool AFortniteCloneCharacter::ServerAimHipFire_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerPickaxeTimeOut_Implementation() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			State->JustSwungPickaxe = false;
		}
	}
}

bool AFortniteCloneCharacter::ServerPickaxeTimeOut_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerRifleTimeOut_Implementation() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			State->JustShotRifle = false;
		}
	}
}

bool AFortniteCloneCharacter::ServerRifleTimeOut_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerShotgunTimeOut_Implementation() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			State->JustShotShotgun = false;
		}
	}
}

bool AFortniteCloneCharacter::ServerShotgunTimeOut_Validate() {
	return true;
}


void AFortniteCloneCharacter::ServerHealingItemTimeOut_Implementation(int HealingItemType) {
	if (HealingItemType == 0) {
		if (Health < 100) {
			if (Health + 15 > 100) {
				Health = 100;
			}
			else {
				Health += 15;
			}
		}
	}
	else if (HealingItemType == 1) {
		if (Shield < 100) {
			if (Shield + 20 > 100) {
				Shield = 100;
			}
			else {
				Shield += 20;
			}
		}
	}
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			State->JustUsedHealingItem = false;
		}
	}
}

bool AFortniteCloneCharacter::ServerHealingItemTimeOut_Validate(int HealingItemType) {
	return true;
}

void AFortniteCloneCharacter::ServerApplyStormDamage_Implementation() {
	if (InStorm) {
		//get storm actor and get its damage component and apply the damage to the player's health
		Health -= CurrentStorm->Damage;
		if (Health <= 0) {
			if (BuildingPreview) {
				BuildingPreview->Destroy();
			}
			if (CurrentWeapon) {
				CurrentWeapon->Destroy();
			}
			if (CurrentHealingItem) {
				CurrentHealingItem->Destroy();
			}
			if (GetController()) {
				AFortniteClonePlayerController* FortniteClonePlayerController = Cast<AFortniteClonePlayerController>(GetController());
				if (FortniteClonePlayerController) {
					FortniteClonePlayerController->ServerSwitchToSpectatorMode();
				}
			}
			Destroy();
		}
	}
}

bool AFortniteCloneCharacter::ServerApplyStormDamage_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerRifleReloadTimeOut_Implementation() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			State->JustReloadedRifle = false;
		}
	}
}

bool AFortniteCloneCharacter::ServerRifleReloadTimeOut_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerShotgunReloadTimeOut_Implementation() {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			State->JustReloadedShotgun = false;
		}
	}
}

bool AFortniteCloneCharacter::ServerShotgunReloadTimeOut_Validate() {
	return true;
}

void AFortniteCloneCharacter::ServerSpawnProjectile_Implementation(FTransform SpawnTransform) {
	
	auto Bullet = Cast<AProjectileActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, CurrentWeapon->BulletClass, SpawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (Bullet != nullptr)
	{
		//spawnactor has no way of passing parameters so need to use begindeferredactorspawn and finishspawningactor
		Bullet->Weapon = CurrentWeapon;
		Bullet->WeaponHolder = this;
		Bullet->SetOwner(this);

		UGameplayStatics::FinishSpawningActor(Bullet, SpawnTransform);
	}
}

bool AFortniteCloneCharacter::ServerSpawnProjectile_Validate(FTransform SpawnTransform) {
	return true;
}

void AFortniteCloneCharacter::ServerSpawnAndAttachWeapon_Implementation(int WeaponType, FTransform SpawnTransform) {
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			CurrentWeaponType = WeaponType;
			CurrentHealingItemType = -1;
			FName WeaponSocketName = TEXT("hand_right_socket_pickaxe");
			if (CurrentWeaponType == 1) {
				WeaponSocketName = TEXT("hand_right_socket_rifle");
			}
			else if (CurrentWeaponType == 2) {
				WeaponSocketName = TEXT("hand_right_socket_shotgun");
			}
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
			CurrentWeapon = Cast<AWeaponActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, WeaponClasses[WeaponType], SpawnTransform));
			if (CurrentWeapon != nullptr)
			{
				CurrentWeapon->CurrentBulletCount = State->EquippedWeaponsClips[CurrentWeaponType];
				//spawnactor has no way of passing parameters so need to use begindeferredactorspawn and finishspawningactor
				CurrentWeapon->Holder = this;
				UGameplayStatics::FinishSpawningActor(CurrentWeapon, SpawnTransform);
			}

			UStaticMeshComponent* WeaponStaticMeshComponent = Cast<UStaticMeshComponent>(CurrentWeapon->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			WeaponStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, WeaponSocketName);

			//animinstance properties
			HoldingWeapon = true;
			AimedIn = false;
			HoldingWeaponType = 1;
			State->HoldingWeapon = true;
			State->HoldingHealingItem = false;
			State->CurrentWeapon = WeaponType;
			State->CurrentHealingItem = -1;
		}
	}
}

bool AFortniteCloneCharacter::ServerSpawnAndAttachWeapon_Validate(int WeaponType, FTransform SpawnTransform) {
	return true;
}

void AFortniteCloneCharacter::ServerSpawnAndAttachHealingItem_Implementation(int HealingItemType, FTransform SpawnTransform) {
	/*FString LogMsg = FString("server spawn and attach Current healing item type ") + FString::FromInt(HealingItemType);
	UE_LOG(LogMyGame, Warning, TEXT("%s"), *LogMsg);*/
	if (GetController()) {
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			CurrentWeaponType = -1;
			CurrentHealingItemType = HealingItemType;
			FName HealingItemSocketName = TEXT("hand_left_socket_bandage");

			if (CurrentHealingItemType == 1) {
				HealingItemSocketName = TEXT("hand_left_socket_potion");
			}

			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, true);

			auto HealingItem = Cast<AHealingActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, HealingItemClasses[CurrentHealingItemType], SpawnTransform));
			if (HealingItem != nullptr)
			{
				CurrentHealingItem = HealingItem;
				//spawnactor has no way of passing parameters so need to use begindeferredactorspawn and finishspawningactor
				CurrentHealingItem->Holder = this;

				UGameplayStatics::FinishSpawningActor(CurrentHealingItem, SpawnTransform);
			}

			UStaticMeshComponent* HealingItemStaticMeshComponent = Cast<UStaticMeshComponent>(CurrentHealingItem->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			HealingItemStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, HealingItemSocketName);

			//animinstance properties
			HoldingWeapon = false;
			AimedIn = false;
			HoldingWeaponType = 0;
			State->HoldingWeapon = false;
			State->HoldingHealingItem = true;
			State->CurrentWeapon = -1;
			State->CurrentHealingItem = HealingItemType;
		}
	}
}

bool AFortniteCloneCharacter::ServerSpawnAndAttachHealingItem_Validate(int HealingItemType, FTransform SpawnTransform) {
	return true;
}

void AFortniteCloneCharacter::ClientCameraAimIn_Implementation() {
	FollowCamera->FieldOfView = 45;
}

void AFortniteCloneCharacter::ClientCameraAimOut_Implementation() {
	FollowCamera->FieldOfView = 90;
}

void AFortniteCloneCharacter::ClientGetBulletTransform_Implementation() {
	FName WeaponSocketName = TEXT("hand_right_socket_pickaxe");
	if (CurrentWeaponType == 1) {
		WeaponSocketName = TEXT("hand_right_socket_rifle");
	}
	else if (CurrentWeaponType == 2) {
		WeaponSocketName = TEXT("hand_right_socket_shotgun");
	}
	if (CurrentWeapon != nullptr) {
		TArray<UStaticMeshComponent*> Components;
		CurrentWeapon->GetComponents(Components);
		// find the muzzle component
		for (int i = 0; i < Components.Num(); i++) {
			if (Components[i]->GetName() == "Weapon") {
				UStaticMeshComponent* WeaponComponent = Components[i];

				FVector MuzzleLocation = WeaponComponent->GetSocketLocation("muzzle");
				//DrawDebugSphere(GetWorld(), MuzzleLocation, 200, 26, FColor(255, 0, 0), true, -1, 0, 2);
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(GetActorRotation().Roll) +FString(" ") + FString::SanitizeFloat(GetActorRotation().Pitch) +FString(" ") + FString::SanitizeFloat(GetActorRotation().Yaw));
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(MuzzleLocation.X) + FString(" ") + FString::SanitizeFloat(MuzzleLocation.Y) + FString(" ") + FString::SanitizeFloat(MuzzleLocation.Z));
				if (GetController() != nullptr) {
					APlayerController* PlayerController = Cast<APlayerController>(GetController());
					FRotator CameraRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
					FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
					PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
					FVector ShootDirection = CameraRotation.Vector();
					const FVector StartTrace = FollowCamera->GetComponentLocation();
					const FVector EndTrace = StartTrace + ShootDirection * 100000.0;
					FHitResult Impact;
					FCollisionQueryParams CollisionParams;
					GetWorld()->LineTraceSingleByChannel(Impact, StartTrace, EndTrace, ECC_Visibility, CollisionParams);

					if (Impact.bBlockingHit) {
						FTransform SpawnTransform(ShootDirection.Rotation(), MuzzleLocation);
						ServerSpawnProjectile(SpawnTransform);
					}
					break;
				}
			}
		}
	}
	
}

void AFortniteCloneCharacter::NetMulticastPlayPickaxeSwingAnimation_Implementation() {
	PlayAnimMontage(PickaxeSwingingAnimation, 1.f, NAME_None);
}

void AFortniteCloneCharacter::NetMulticastPlayShootRifleAnimation_Implementation() {
	PlayAnimMontage(RifleHipShootingAnimation);
}

void AFortniteCloneCharacter::NetMulticastPlayShootShotgunAnimation_Implementation() {
	PlayAnimMontage(ShotgunHipShootingAnimation);
}

void AFortniteCloneCharacter::NetMulticastPlayShootRifleIronsightsAnimation_Implementation() {
	PlayAnimMontage(RifleIronsightsShootingAnimation);
}

void AFortniteCloneCharacter::NetMulticastPlayShootShotgunIronsightsAnimation_Implementation() {
	PlayAnimMontage(ShotgunIronsightsShootingAnimation);
}

void AFortniteCloneCharacter::NetMulticastPlayUseHealingItemAnimation_Implementation() {
	PlayAnimMontage(HealingAnimation);
}

void AFortniteCloneCharacter::NetMulticastPlayReloadRifleAnimation_Implementation() {
	PlayAnimMontage(RifleHipReloadAnimation);
}

void AFortniteCloneCharacter::NetMulticastPlayReloadRifleIronsightsAnimation_Implementation() {
	PlayAnimMontage(RifleIronsightsReloadAnimation);
}

void AFortniteCloneCharacter::NetMulticastPlayReloadShotgunAnimation_Implementation() {
	PlayAnimMontage(ShotgunHipReloadAnimation);
}

void AFortniteCloneCharacter::NetMulticastPlayReloadShotgunIronsightsAnimation_Implementation() {
	PlayAnimMontage(ShotgunIronsightsReloadAnimation);
}

void AFortniteCloneCharacter::ClientDrawHitMarker_Implementation() {
	if (GetController()) {
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController->GetHUD()) {
			AFortniteCloneHUD* FortniteCloneHUD = Cast<AFortniteCloneHUD>(PlayerController->GetHUD());
			FortniteCloneHUD->DrawHitMarker();
		}
	}
}

void AFortniteCloneCharacter::ClientDrawBloodEffect_Implementation() {
	if (GetController()) {
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController->GetHUD()) {
			AFortniteCloneHUD* FortniteCloneHUD = Cast<AFortniteCloneHUD>(PlayerController->GetHUD());
			FortniteCloneHUD->DrawBloodSplash();
		}
	}
}

void AFortniteCloneCharacter::ClientDestroyStructure_Implementation(int StructureId) {
	for (int i = 0; i < ClientSpawnedStructures.Num(); i++) {
		if (ClientSpawnedStructures[i] != nullptr) {
			if (ClientSpawnedStructures[i]->Id == StructureId) {
				ABuildingActor* Structure = ClientSpawnedStructures[i];
				ClientSpawnedStructures.Remove(Structure);
				Structure->Destroy();
				break;
			}
		}
		
	}
}
void AFortniteCloneCharacter::ClientGetWeaponTransform_Implementation(int WeaponType) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("ClientGetweapontransform"));
	FTransform SpawnTransform(GetActorRotation(), GetActorLocation());
	ServerSpawnAndAttachWeapon(WeaponType, SpawnTransform);
}

void AFortniteCloneCharacter::ClientGetHealingItemTransform_Implementation(int HealingItemType) {
	/*FString LogMsg = FString("client get healing item transform Current healing item type ") + FString::FromInt(HealingItemType);
	UE_LOG(LogMyGame, Warning, TEXT("%s"), *LogMsg);*/
	FTransform SpawnTransform(GetActorRotation(), GetActorLocation());
	ServerSpawnAndAttachHealingItem(HealingItemType, SpawnTransform);
}

void AFortniteCloneCharacter::OnRepSetSkin() {
	USkeletalMeshComponent* CharacterMesh = GetMesh();
	if (MaterialOrMaterialInstance == 0) {
		// assign UMaterial
		CharacterMesh->SetMaterial(0, SkinMaterials[CurrentSkin]);
	}
	else {
		// assign UMaterialInstance
		CharacterMesh->SetMaterial(0, SkinMaterialInstances[CurrentSkin]);
	}
}