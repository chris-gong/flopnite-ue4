// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FortniteCloneCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine.h"
#include "WeaponActor.h"
#include "FortniteClonePlayerState.h"
#include "BuildingActor.h"
#include "ThirdPersonAnimInstance.h"
#include "ProjectileActor.h"
#include "HealingActor.h"
#include "AmmunitionActor.h"
#include "UnrealNetwork.h"
#include "Engine/ActorChannel.h"

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
	CurrentBuildingMaterial = 0;
	BuildingPreview = nullptr;
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
	PlayerInputComponent->BindAction("BuildStructure", IE_Pressed, this, &AFortniteCloneCharacter::BuildStructure);
	PlayerInputComponent->BindAction("SwitchBuildingMaterial", IE_Pressed, this, &AFortniteCloneCharacter::SwitchBuildingMaterial);
	PlayerInputComponent->BindAction("ShootGun", IE_Pressed, this, &AFortniteCloneCharacter::ShootGun);
	PlayerInputComponent->BindAction("UseBandage", IE_Pressed, this, &AFortniteCloneCharacter::UseBandage);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFortniteCloneCharacter::Reload);
	PlayerInputComponent->BindAction("HoldPickaxe", IE_Pressed, this, &AFortniteCloneCharacter::HoldPickaxe);
	PlayerInputComponent->BindAction("HoldAssaultRifle", IE_Pressed, this, &AFortniteCloneCharacter::HoldAssaultRifle);
	PlayerInputComponent->BindAction("HoldShotgun", IE_Pressed, this, &AFortniteCloneCharacter::HoldShotgun);
	PlayerInputComponent->BindAction("HoldBandage", IE_Pressed, this, &AFortniteCloneCharacter::HoldBandage);
	PlayerInputComponent->BindAction("Ironsights", IE_Pressed, this, &AFortniteCloneCharacter::AimGunIn);
	PlayerInputComponent->BindAction("Ironsights", IE_Released, this, &AFortniteCloneCharacter::AimGunOut);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFortniteCloneCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFortniteCloneCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AFortniteCloneCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AFortniteCloneCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AFortniteCloneCharacter::OnResetVR);
}

void AFortniteCloneCharacter::BeginPlay() {
	Super::BeginPlay();
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(GetNetMode()));
	/*if (GetNetMode() != ENetMode::NM_Client || GetNetMode() != ENetMode::NM_Standalone) {
		return;
	}*/
	if (HasAuthority()) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(GetNetMode()) + FString("I have da authoritay"));
		//AnimInstance = NewObject<UThirdPersonAnimInstance>(this);
		GetMesh()->SetAnimInstanceClass(AnimInstanceClass);
		if (WeaponClasses[CurrentWeaponType]) {
			FName WeaponSocketName = TEXT("hand_right_socket");
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
			CurrentWeapon = GetWorld()->SpawnActor<AWeaponActor>(WeaponClasses[CurrentWeaponType], GetActorLocation(), GetActorRotation());
			UStaticMeshComponent* WeaponStaticMeshComponent = Cast<UStaticMeshComponent>(CurrentWeapon->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			WeaponStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, WeaponSocketName);
			CurrentWeapon->Holder = this;
			if (GetMesh()) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(GetNetMode()) + FString(" mesh exists"));
				AnimInstance = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
				//AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
				if (AnimInstance) {
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(GetNetMode()) + FString(" animinstance exists"));
					/*AnimInstance->HoldingWeapon = true;
					AnimInstance->AimedIn = false;
					AnimInstance->HoldingWeaponType = 1;*/
					//State->HoldingWeapon = true;
					//State->CurrentWeapon = 0;
				}
			}

		}
		if (GetController()) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(GetNetMode()) + FString(" controller is not null"));
		}
	}
	//set input to ui only for the main screen level
	//Cast<APlayerController>(GetController())->SetInputMode(FInputModeGameAndUI());
	/*if (WeaponClasses[CurrentWeaponType]) {
		FName WeaponSocketName = TEXT("hand_right_socket");
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponActor>(WeaponClasses[CurrentWeaponType], GetActorLocation(), GetActorRotation());
		UStaticMeshComponent* WeaponStaticMeshComponent = Cast<UStaticMeshComponent>(CurrentWeapon->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		WeaponStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, WeaponSocketName);
		CurrentWeapon->Holder = this;

		UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
		//AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (Animation) {
			Animation->HoldingWeapon = true;
			Animation->AimedIn = false;
			Animation->HoldingWeaponType = 1;
			//State->HoldingWeapon = true;
			//State->CurrentWeapon = 0;
		}
	}*/
	if (GetController()) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("controller is not null"));
		//set input to ui only for the main screen level <- don't do this, preventing aim offset from happening unless left mouse is held down
		//Cast<APlayerController>(GetController())->SetInputMode(FInputModeGameAndUI());
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (State) {
			State->HoldingWeapon = true;
			State->CurrentWeapon = 0;
		}
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
	if (AnimInstance != nullptr)
	{
		WroteSomething |= Channel->ReplicateSubobject(AnimInstance, *Bunch, *RepFlags);
	}

	return WroteSomething;
}

void AFortniteCloneCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFortniteCloneCharacter, AnimInstance);
}

void AFortniteCloneCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	GetCapsuleComponent()->SetWorldRotation(GetCameraBoom()->GetComponentRotation());
	UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());

	if (Animation) {
		
		FVector DirectionVector = FVector(0, Animation->AimYaw, Animation->AimPitch);
		if (GetController()) {
			AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
			if (State) {
				if (State->InBuildMode && State->BuildMode == FString("Wall")) {
					if (BuildingPreview) {
						BuildingPreview->Destroy(); //destroy the last wall preview
					}
					FString LogMsg = FString("Current building material ") + FString::FromInt(CurrentBuildingMaterial);
					UE_LOG(LogMyGame, Warning, TEXT("%s"), *LogMsg);
					if (CurrentBuildingMaterial >= 0 && CurrentBuildingMaterial <= 2) {
						if (WallPreviewClasses.IsValidIndex(CurrentBuildingMaterial)) {
							if (WallPreviewClasses[CurrentBuildingMaterial] != nullptr) {
								BuildingPreview = GetWorld()->SpawnActor<ABuildingActor>(WallPreviewClasses[CurrentBuildingMaterial], GetActorLocation() + (GetActorForwardVector() * 200) + (DirectionVector * 3), GetActorRotation().Add(0, 90, 0)); //set the new wall preview
							}
						}
					}
				}
				if (State->InBuildMode && State->BuildMode == FString("Ramp")) {
					if (BuildingPreview) {
						BuildingPreview->Destroy(); //destroy the last wall preview
					}
					FString LogMsg = FString("Current building material ") + FString::FromInt(CurrentBuildingMaterial);
					UE_LOG(LogMyGame, Warning, TEXT("%s"), *LogMsg);
					if (CurrentBuildingMaterial >= 0 && CurrentBuildingMaterial <= 2) {
						if (RampPreviewClasses.IsValidIndex(CurrentBuildingMaterial)) {
							if (RampPreviewClasses[CurrentBuildingMaterial] != nullptr) {
								BuildingPreview = GetWorld()->SpawnActor<ABuildingActor>(RampPreviewClasses[CurrentBuildingMaterial], GetActorLocation() + (GetActorForwardVector() * 100) + (DirectionVector * 3), GetActorRotation().Add(0, 90, 0)); //set the new ramp preview
							}
						}
					}
				}
				if (State->InBuildMode && State->BuildMode == FString("Floor")) {
					if (BuildingPreview) {
						BuildingPreview->Destroy(); //destroy the last wall preview
					}
					FString LogMsg = FString("Current building material ") + FString::FromInt(CurrentBuildingMaterial);
					UE_LOG(LogMyGame, Warning, TEXT("%s"), *LogMsg);
					if (CurrentBuildingMaterial >= 0 && CurrentBuildingMaterial <= 2) {
						if (FloorPreviewClasses.IsValidIndex(CurrentBuildingMaterial)) {
							if (FloorPreviewClasses[CurrentBuildingMaterial] != nullptr) {
								BuildingPreview = GetWorld()->SpawnActor<ABuildingActor>(FloorPreviewClasses[CurrentBuildingMaterial], GetActorLocation() + (GetActorForwardVector() * 120) + (DirectionVector * 3), GetActorRotation().Add(0, 90, 0)); //set the new floor preview
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

		FRotator AimRotation = FRotator(Animation->AimPitch, Animation->AimYaw, 0);
		FRotator InterpolatedRotation = FMath::RInterpTo(AimRotation, DeltaRotation, DeltaTime, Animation->InterpSpeed);

		float NewPitch = FMath::ClampAngle(InterpolatedRotation.Pitch, -90, 90);
		float NewYaw = FMath::ClampAngle(InterpolatedRotation.Yaw, -90, 90);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(NewPitch).Append(FString::FromInt(NewYaw)));
		Animation->AimPitch = NewPitch;
		Animation->AimYaw = NewYaw;
	}
	
}

void AFortniteCloneCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor != nullptr && OtherActor != this) {
		if (CurrentWeapon != nullptr && OtherActor == (AActor*) CurrentWeapon) {
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
				if (State->InBuildMode || State->JustShotRifle || State->JustShotShotgun || State->JustSwungPickaxe || State->JustUsedBandage || State->JustReloadedRifle || State->JustReloadedShotgun) {
					return; // can't pick up items while in build mode or if just shot rifle, shot shotgun, swung pickaxe, used bandage, or reloaded
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
				FName BandageSocketName = TEXT("hand_right_socket");
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);

				CurrentWeapon = WeaponActor;
				CurrentWeaponType = WeaponActor->WeaponType;
				CurrentWeapon->Holder = this;
				int MagazineSize = CurrentWeapon->MagazineSize;
				CurrentWeapon->CurrentBulletCount = MagazineSize;
				UStaticMeshComponent* OutHitStaticMeshComponent = Cast<UStaticMeshComponent>(WeaponActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
				OutHitStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, BandageSocketName);

				if (State) {
					State->HoldingWeapon = true;
					State->HoldingBandage = false;
					State->EquippedWeapons.Add(WeaponActor->WeaponType);
					State->CurrentWeapon = WeaponActor->WeaponType;
					State->EquippedWeaponsClips[CurrentWeaponType] = MagazineSize;
					UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
					if (Animation) {
						Animation->HoldingWeapon = true;
						Animation->HoldingWeaponType = 1;
					}
				}
			}
			
		}
		else if (OtherActor->IsA(AHealingActor::StaticClass())) {
			//pick up the item
			AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
			if (CurrentHealingItem) {
				CurrentHealingItem->Destroy();
				CurrentHealingItem = nullptr;
			}
			
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, OtherActor->GetName());
			if (State->InBuildMode || State->JustShotRifle || State->JustShotShotgun || State->JustSwungPickaxe || State->JustUsedBandage || State->JustReloadedRifle || State->JustReloadedShotgun) {
				return; // can't pick up items while in build mode or if just shot rifle, shot shotgun, swung pickaxe, used bandage, or reloaded
			}
			CurrentHealingItem = Cast<AHealingActor>(OtherActor);
			if (CurrentHealingItem->Holder != nullptr) {
				return; // do nothing if someone is holding the weapon
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
			// PICK UP BANDAGE 
			FName BandageSocketName = TEXT("hand_left_socket");
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);

			CurrentWeapon = nullptr;
			CurrentWeaponType = -1;
			CurrentHealingItem->Holder = this;
			UStaticMeshComponent* OutHitStaticMeshComponent = Cast<UStaticMeshComponent>(CurrentHealingItem->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			OutHitStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, BandageSocketName);

			if (State) {
				State->HoldingWeapon = false;
				State->HoldingBandage = true;
				State->BandageCount++;
				State->CurrentWeapon = -1;
				UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
				if (Animation) {
					Animation->HoldingWeapon = false;
					Animation->HoldingWeaponType = 0;
				}
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
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, OtherActor->GetName());
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
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(Controller->PlayerState);
	if (State) {
		if (State->JustUsedBandage) {
			return;
		}
	}
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
	UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
	if (Animation) {
		//set blend space variable
		Animation->WalkingY = Value * 90;
		Animation->RunningY = Value * 90;
	}
}

void AFortniteCloneCharacter::MoveRight(float Value)
{
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (State) {
		if (State->JustUsedBandage) {
			return;
		}
	}
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
	UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
	if (Animation) {
		//set blend space variable
		Animation->WalkingX = Value * 90;
		Animation->RunningX = Value * 90;
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
			GetCharacterMovement()->MaxWalkSpeed = 200.0;
		}
		else if (Value == 0) {
			GetCharacterMovement()->MaxWalkSpeed = 450.0;
			Animation->IsRunning = false;
		}
		else {
			// can only sprint if the w key is held down by itself or in combination with the a or d keys
			if (!(OnlyAOrDDown || SDown) && WDown) {
				GetCharacterMovement()->MaxWalkSpeed = 900.0;
				Animation->IsRunning = true;
			}
			else {
				GetCharacterMovement()->MaxWalkSpeed = 450.0;
				Animation->IsRunning = false;
			}
		}
	}
}

void AFortniteCloneCharacter::StartWalking() {
	UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
	if (Animation) {
		Animation->IsWalking = true;
	}
}

void AFortniteCloneCharacter::StopWalking() {
	UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
	APlayerController* LocalController = Cast<APlayerController>(GetController());
	bool ADown = LocalController->IsInputKeyDown(EKeys::A);
	bool WDown = LocalController->IsInputKeyDown(EKeys::W);
	bool SDown = LocalController->IsInputKeyDown(EKeys::S);
	bool DDown = LocalController->IsInputKeyDown(EKeys::D);
	bool NoWalkingKeysDown = !ADown && !WDown && !SDown && !DDown;

	if (Animation && NoWalkingKeysDown) {
		Animation->IsWalking = false;
	}
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
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "x key pressed");
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (State) {
		if (State->JustUsedBandage || State->JustReloadedRifle || State->JustReloadedShotgun) {
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
				FName WeaponSocketName = TEXT("hand_right_socket");
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);

				FTransform SpawnTransform(GetActorRotation(), GetActorLocation());
				CurrentWeapon = Cast<AWeaponActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, WeaponClasses[CurrentWeaponType], SpawnTransform));
				if (CurrentWeapon != nullptr)
				{
					//spawnactor has no way of passing parameters so need to use begindeferredactorspawn and finishspawningactor
					CurrentWeapon->Holder = this;
					if (CurrentWeaponType > 0 && CurrentWeaponType < 3) {
						CurrentWeapon->CurrentBulletCount = State->EquippedWeaponsClips[CurrentWeaponType];
					}
					UGameplayStatics::FinishSpawningActor(CurrentWeapon, SpawnTransform);
				}

				UStaticMeshComponent* WeaponStaticMeshComponent = Cast<UStaticMeshComponent>(CurrentWeapon->GetComponentByClass(UStaticMeshComponent::StaticClass()));
				WeaponStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, WeaponSocketName);
				
				UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
				if (Animation) {
					Animation->HoldingWeapon = true;
					Animation->AimedIn = false;
					State->HoldingWeapon = true;
					State->HoldingBandage = false;
					State->CurrentWeapon = CurrentWeaponType;
					Animation->HoldingWeaponType = 1;
				}
			}
			else {
				//equip bandage since current weapon was null
				FName BandageSocketName = TEXT("hand_left_socket");
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);

				FTransform SpawnTransform(GetActorRotation(), GetActorLocation());
				auto CurrentHealingItem = Cast<AHealingActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, BandageClass, SpawnTransform));
				if (CurrentHealingItem != nullptr)
				{
					//spawnactor has no way of passing parameters so need to use begindeferredactorspawn and finishspawningactor
					CurrentHealingItem->Holder = this;

					UGameplayStatics::FinishSpawningActor(CurrentHealingItem, SpawnTransform);
				}

				UStaticMeshComponent* HealingItemStaticMeshComponent = Cast<UStaticMeshComponent>(CurrentHealingItem->GetComponentByClass(UStaticMeshComponent::StaticClass()));
				HealingItemStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, BandageSocketName);

				UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
				if (Animation) {
					Animation->HoldingWeapon = false;
					Animation->AimedIn = false;
					State->HoldingWeapon = false;
					State->HoldingBandage = true;
					Animation->HoldingWeaponType = 0;
				}
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
			State->HoldingBandage = false;
			State->AimedIn = false;
			UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
			if (Animation) {
				Animation->HoldingWeapon = false;
				Animation->AimedIn = false;
				Animation->HoldingWeaponType = 0;
			}
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

void AFortniteCloneCharacter::PreviewRamp() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "c key pressed");
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "c key presse2d");
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (State) {
		if (State->JustUsedBandage || State->JustReloadedRifle || State->JustReloadedShotgun) {
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
				FName WeaponSocketName = TEXT("hand_right_socket");
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);

				FTransform SpawnTransform(GetActorRotation(), GetActorLocation());
				CurrentWeapon = Cast<AWeaponActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, WeaponClasses[CurrentWeaponType], SpawnTransform));
				if (CurrentWeapon != nullptr)
				{
					//spawnactor has no way of passing parameters so need to use begindeferredactorspawn and finishspawningactor
					CurrentWeapon->Holder = this;
					if (CurrentWeaponType > 0 && CurrentWeaponType < 3) {
						CurrentWeapon->CurrentBulletCount = State->EquippedWeaponsClips[CurrentWeaponType];
					}
					UGameplayStatics::FinishSpawningActor(CurrentWeapon, SpawnTransform);
				}

				UStaticMeshComponent* WeaponStaticMeshComponent = Cast<UStaticMeshComponent>(CurrentWeapon->GetComponentByClass(UStaticMeshComponent::StaticClass()));
				WeaponStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, WeaponSocketName);

				UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
				if (Animation) {
					Animation->HoldingWeapon = true;
					Animation->AimedIn = false;
					State->HoldingWeapon = true;
					State->HoldingBandage = false;
					State->CurrentWeapon = CurrentWeaponType;
					Animation->HoldingWeaponType = 1;
				}
			}
			else {
				//equip bandage since current weapon was null
				FName BandageSocketName = TEXT("hand_left_socket");
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);

				FTransform SpawnTransform(GetActorRotation(), GetActorLocation());
				auto CurrentHealingItem = Cast<AHealingActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, BandageClass, SpawnTransform));
				if (CurrentHealingItem != nullptr)
				{
					//spawnactor has no way of passing parameters so need to use begindeferredactorspawn and finishspawningactor
					CurrentHealingItem->Holder = this;

					UGameplayStatics::FinishSpawningActor(CurrentHealingItem, SpawnTransform);
				}

				UStaticMeshComponent* HealingItemStaticMeshComponent = Cast<UStaticMeshComponent>(CurrentHealingItem->GetComponentByClass(UStaticMeshComponent::StaticClass()));
				HealingItemStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, BandageSocketName);

				UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
				if (Animation) {
					Animation->HoldingWeapon = false;
					Animation->AimedIn = false;
					State->HoldingWeapon = false;
					State->HoldingBandage = true;
					Animation->HoldingWeaponType = 0;
				}
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
			State->HoldingBandage = false;
			State->AimedIn = false;
			UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
			if (Animation) {
				Animation->HoldingWeapon = false;
				Animation->AimedIn = false;
				Animation->HoldingWeaponType = 0;
			}
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

void AFortniteCloneCharacter::PreviewFloor() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "f key pressed");
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (State) {
		if (State->JustUsedBandage || State->JustReloadedRifle || State->JustReloadedShotgun || State->JustSwungPickaxe) {
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
				FName WeaponSocketName = TEXT("hand_right_socket");
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);

				FTransform SpawnTransform(GetActorRotation(), GetActorLocation());
				CurrentWeapon = Cast<AWeaponActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, WeaponClasses[CurrentWeaponType], SpawnTransform));
				if (CurrentWeapon != nullptr)
				{
					//spawnactor has no way of passing parameters so need to use begindeferredactorspawn and finishspawningactor
					CurrentWeapon->Holder = this;
					if (CurrentWeaponType > 0 && CurrentWeaponType < 3) {
						CurrentWeapon->CurrentBulletCount = State->EquippedWeaponsClips[CurrentWeaponType];
					}
					UGameplayStatics::FinishSpawningActor(CurrentWeapon, SpawnTransform);
				}

				UStaticMeshComponent* WeaponStaticMeshComponent = Cast<UStaticMeshComponent>(CurrentWeapon->GetComponentByClass(UStaticMeshComponent::StaticClass()));
				WeaponStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, WeaponSocketName);

				UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
				if (Animation) {
					Animation->HoldingWeapon = true;
					Animation->AimedIn = false;
					Animation->HoldingWeaponType = 1;
					State->HoldingWeapon = true;
					State->HoldingBandage = false;
					State->CurrentWeapon = CurrentWeaponType;
					Animation->HoldingWeaponType = 1;
				}
			}
			else {
				//equip bandage since current weapon was null
				FName BandageSocketName = TEXT("hand_left_socket");
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);

				FTransform SpawnTransform(GetActorRotation(), GetActorLocation());
				auto CurrentHealingItem = Cast<AHealingActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, BandageClass, SpawnTransform));
				if (CurrentHealingItem != nullptr)
				{
					//spawnactor has no way of passing parameters so need to use begindeferredactorspawn and finishspawningactor
					CurrentHealingItem->Holder = this;

					UGameplayStatics::FinishSpawningActor(CurrentHealingItem, SpawnTransform);
				}
				
				UStaticMeshComponent* HealingItemStaticMeshComponent = Cast<UStaticMeshComponent>(CurrentHealingItem->GetComponentByClass(UStaticMeshComponent::StaticClass()));
				HealingItemStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, BandageSocketName);

				UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
				if (Animation) {
					Animation->HoldingWeapon = false;
					Animation->AimedIn = false;
					State->HoldingWeapon = false;
					State->HoldingBandage = true;
					Animation->HoldingWeaponType = 0;
				}
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
			State->HoldingBandage = false;
			State->AimedIn = false;
			UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
			if (Animation) {
				Animation->HoldingWeapon = false;
				Animation->AimedIn = false;
				Animation->HoldingWeaponType = 0;
			}
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

void AFortniteCloneCharacter::BuildStructure() {
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
	if (State) {
		FVector DirectionVector = FVector(0, Animation->AimYaw, Animation->AimPitch);
		if (State->InBuildMode && State->BuildMode == FString("Wall") && State->MaterialCounts[CurrentBuildingMaterial] >= 10) {
			TArray<AActor*> OverlappingActors;
			ABuildingActor* Wall = GetWorld()->SpawnActor<ABuildingActor>(WallClasses[CurrentBuildingMaterial], GetActorLocation() + (GetActorForwardVector() * 200) + (DirectionVector * 3), GetActorRotation().Add(0, 90, 0));

			Wall->GetOverlappingActors(OverlappingActors);

			for (int i = 0; i < OverlappingActors.Num(); i++) {
				//don't allow a player to build a structure that overlaps with another player
				if (OverlappingActors[i]->IsA(AFortniteCloneCharacter::StaticClass())) {
					Wall->Destroy();
					return;
				}
			}
			State->MaterialCounts[CurrentBuildingMaterial] -= 10;
		}
		else if (State->InBuildMode && State->BuildMode == FString("Ramp") && State->MaterialCounts[CurrentBuildingMaterial] >= 10) {
			TArray<AActor*> OverlappingActors;

			ABuildingActor* Ramp = GetWorld()->SpawnActor<ABuildingActor>(RampClasses[CurrentBuildingMaterial], GetActorLocation() + (GetActorForwardVector() * 100) + (DirectionVector * 3), GetActorRotation().Add(0, 90, 0));

			Ramp->GetOverlappingActors(OverlappingActors);

			for (int i = 0; i < OverlappingActors.Num(); i++) {
				//don't allow a player to build a structure that overlaps with another player
				if (OverlappingActors[i]->IsA(AFortniteCloneCharacter::StaticClass())) {
					Ramp->Destroy();
					return;
				}
			}
			State->MaterialCounts[CurrentBuildingMaterial] -= 10;
		}
		else if (State->InBuildMode && State->BuildMode == FString("Floor") && State->MaterialCounts[CurrentBuildingMaterial] >= 10) {
			TArray<AActor*> OverlappingActors;
			ABuildingActor* Floor = GetWorld()->SpawnActor<ABuildingActor>(FloorClasses[CurrentBuildingMaterial], GetActorLocation() + (GetActorForwardVector() * 120) + (DirectionVector * 3), GetActorRotation().Add(0, 90, 0));

			Floor->GetOverlappingActors(OverlappingActors);

			for (int i = 0; i < OverlappingActors.Num(); i++) {
				//don't allow a player to build a structure that overlaps with another player
				if (OverlappingActors[i]->IsA(AFortniteCloneCharacter::StaticClass())) {
					Floor->Destroy();
					return;
				}
			}
			State->MaterialCounts[CurrentBuildingMaterial] -= 10;
		}
	}
}

void AFortniteCloneCharacter::SwitchBuildingMaterial() {
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

void AFortniteCloneCharacter::ShootGun() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(GetNetMode()) + FString(" shoot gun key pressed"));
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (State) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(GetNetMode()) + FString(" Current weapon ") + FString::FromInt(State->CurrentWeapon));
		if (State->HoldingWeapon) {
			if (State->CurrentWeapon > 0 && State->CurrentWeapon < 3 && CurrentWeapon->CurrentBulletCount <= 0) {
				// no bullets in magazine, need to reload
				Reload();
				return; 
			}
			if (State->JustReloadedRifle || State->JustReloadedShotgun) {
				return; //currently reloading
			}
			UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
			if (Animation) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(GetNetMode()) + FString("Animation"));
				if (State->AimedIn) {
					if (State->CurrentWeapon == 1) {
						if (State->JustShotRifle) {
							return;
						}
						PlayAnimMontage(RifleIronsightsShootingAnimation);
						CurrentWeapon->CurrentBulletCount--;
						State->EquippedWeaponsClips[CurrentWeaponType]--;
						State->JustShotRifle = true;
						FTimerHandle RifleTimerHandle;
						GetWorldTimerManager().SetTimer(RifleTimerHandle, this, &AFortniteCloneCharacter::RifleTimeOut, 0.233f, false);
					}
					else if (State->CurrentWeapon == 2) {
						if (State->JustShotShotgun) {
							return;
						}
						PlayAnimMontage(ShotgunIronsightsShootingAnimation);
						CurrentWeapon->CurrentBulletCount--;
						State->EquippedWeaponsClips[CurrentWeaponType]--;
						State->JustShotShotgun = true;
						FTimerHandle ShotgunTimerHandle;
						GetWorldTimerManager().SetTimer(ShotgunTimerHandle, this, &AFortniteCloneCharacter::ShotgunTimeOut, 1.3f, false);
					}
				}
				else {
					if (State->CurrentWeapon == 0) {
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "pickaxe swung");
						if (State->JustSwungPickaxe) {
							return;
						}
						PlayAnimMontage(PickaxeSwingingAnimation, 1.f, NAME_None);
						State->JustSwungPickaxe = true;
						FTimerHandle PickaxeTimerHandle;
						GetWorldTimerManager().SetTimer(PickaxeTimerHandle, this, &AFortniteCloneCharacter::PickaxeTimeOut, 0.403f, false);
					}
					if (State->CurrentWeapon == 1) {
						if (State->JustShotRifle) {
							return;
						}
						PlayAnimMontage(RifleHipShootingAnimation);
						CurrentWeapon->CurrentBulletCount--;
						State->EquippedWeaponsClips[CurrentWeaponType]--;
						State->JustShotRifle = true;
						FTimerHandle RifleTimerHandle;
						GetWorldTimerManager().SetTimer(RifleTimerHandle, this, &AFortniteCloneCharacter::RifleTimeOut, 0.233f, false);
					}
					else if (State->CurrentWeapon == 2) {
						if (State->JustShotShotgun) {
							return;
						}
						PlayAnimMontage(ShotgunHipShootingAnimation);
						CurrentWeapon->CurrentBulletCount--;
						State->EquippedWeaponsClips[CurrentWeaponType]--;
						State->JustShotShotgun = true;
						FTimerHandle ShotgunTimerHandle;
						GetWorldTimerManager().SetTimer(ShotgunTimerHandle, this, &AFortniteCloneCharacter::ShotgunTimeOut, 1.3f, false);
					}

				}
				FName WeaponSocketName = TEXT("hand_right_socket");
				FVector BulletLocation = GetMesh()->GetSocketLocation(WeaponSocketName);
				FRotator BulletRotation = GetMesh()->GetSocketRotation(WeaponSocketName);
				/*if (State->CurrentWeapon == 0) {
					BulletRotation = GetActorRotation();
				}*/
				UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
				if (Animation) {
					FVector DirectionVector = FVector(0, Animation->AimYaw * 70, Animation->AimPitch * 20);
					FRotator DirectionRotation = FRotator(BulletRotation.Pitch, GetActorRotation().Yaw, BulletRotation.Roll);
					FTransform SpawnTransform(BulletRotation + FRotator(1,-0.5,0), BulletLocation);
					auto Bullet = Cast<AProjectileActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, CurrentWeapon->BulletClass, SpawnTransform));
					if (Bullet != nullptr)
					{
						//spawnactor has no way of passing parameters so need to use begindeferredactorspawn and finishspawningactor
						Bullet->Weapon = CurrentWeapon;
						Bullet->WeaponHolder = this;

						UGameplayStatics::FinishSpawningActor(Bullet, SpawnTransform);
					}
				}
			}

		}
	}
}

void AFortniteCloneCharacter::UseBandage() {
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (State && State->HoldingBandage) {
		if (State->BandageCount < 1) {
			return; // player has no bandages to use
		}
		UAnimInstance* Animation = GetMesh()->GetAnimInstance();
		UThirdPersonAnimInstance* AnimationInstance = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
		if (Animation && AnimationInstance) {
			if (State->JustUsedBandage) {
				return;
			}
			PlayAnimMontage(HealingAnimation);
			State->JustUsedBandage = true;
			State->BandageCount--;
			FTimerHandle BandageTimerHandle;
			GetWorldTimerManager().SetTimer(BandageTimerHandle, this, &AFortniteCloneCharacter::BandageTimeOut, 3.321f, false);
		}
	}
}

void AFortniteCloneCharacter::Reload() {
	UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (Animation && State) {
		if (State->CurrentWeapon > 2 || State->CurrentWeapon < 1) {
			return; // can only reload if holding a assault rifle or shotgun
		}
		if (State->JustShotRifle || State->JustShotShotgun || State->JustReloadedRifle || State->JustReloadedShotgun) {
			return; // currently reloading or just shot
		}
		UAnimInstance* Animation = GetMesh()->GetAnimInstance();
		UThirdPersonAnimInstance* AnimationInstance = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
		if (Animation && AnimationInstance) {
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
					else{
						State->EquippedWeaponsAmmunition[State->CurrentWeapon] -= BulletsNeeded;
					}
					PlayAnimMontage(RifleIronsightsReloadAnimation);
					CurrentWeapon->CurrentBulletCount += BulletsNeeded;
					State->EquippedWeaponsClips[State->CurrentWeapon] += BulletsNeeded;
					State->JustReloadedRifle = true;
					FTimerHandle RifleTimerHandle;
					GetWorldTimerManager().SetTimer(RifleTimerHandle, this, &AFortniteCloneCharacter::RifleReloadTimeOut, 2.167f, false);
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
					PlayAnimMontage(ShotgunIronsightsReloadAnimation);
					CurrentWeapon->CurrentBulletCount += BulletsNeeded;
					State->EquippedWeaponsClips[State->CurrentWeapon] += BulletsNeeded;
					State->JustReloadedShotgun = true;
					FTimerHandle ShotgunTimerHandle;
					GetWorldTimerManager().SetTimer(ShotgunTimerHandle, this, &AFortniteCloneCharacter::ShotgunReloadTimeOut, 4.3f, false);
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
					PlayAnimMontage(RifleHipReloadAnimation);
					CurrentWeapon->CurrentBulletCount += BulletsNeeded;
					State->EquippedWeaponsClips[State->CurrentWeapon] += BulletsNeeded;
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(CurrentWeapon->CurrentBulletCount));
					State->JustReloadedRifle = true;
					FTimerHandle RifleTimerHandle;
					GetWorldTimerManager().SetTimer(RifleTimerHandle, this, &AFortniteCloneCharacter::RifleReloadTimeOut, 2.167f, false);
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
					PlayAnimMontage(ShotgunHipReloadAnimation);
					CurrentWeapon->CurrentBulletCount += BulletsNeeded;
					State->EquippedWeaponsClips[State->CurrentWeapon] += BulletsNeeded;
					State->JustReloadedShotgun = true;
					FTimerHandle ShotgunTimerHandle;
					GetWorldTimerManager().SetTimer(ShotgunTimerHandle, this, &AFortniteCloneCharacter::ShotgunReloadTimeOut, 4.3f, false);
				}

			}
		}
	}
}

void AFortniteCloneCharacter::AimGunIn() {
	UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (Animation && Animation->HoldingWeapon && State && State->HoldingWeapon && State->CurrentWeapon != 0) {
		Animation->AimedIn = true;
		Animation->HoldingWeaponType = 2;
		CameraBoom->TargetArmLength = 100;
		GetCharacterMovement()->MaxWalkSpeed = 200.0;
		State->AimedIn = true;
	}
}

void AFortniteCloneCharacter::AimGunOut() {
	UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (Animation && Animation->HoldingWeapon && State && State->HoldingWeapon && State->CurrentWeapon != 0) {
		Animation->AimedIn = false;
		Animation->HoldingWeaponType = 1;
		CameraBoom->TargetArmLength = 300;
		GetCharacterMovement()->MaxWalkSpeed = 450.0;
		State->AimedIn = false;
	}
}

void AFortniteCloneCharacter::PickaxeTimeOut() {
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	State->JustSwungPickaxe = false;
}

void AFortniteCloneCharacter::RifleTimeOut() {
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	State->JustShotRifle = false;
}

void AFortniteCloneCharacter::ShotgunTimeOut() {
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	State->JustShotShotgun = false;
}

void AFortniteCloneCharacter::RifleReloadTimeOut() {
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	State->JustReloadedRifle= false;
}

void AFortniteCloneCharacter::ShotgunReloadTimeOut() {
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	State->JustReloadedShotgun = false;
}


void AFortniteCloneCharacter::BandageTimeOut() {
	if (Health < 100) {
		if (Health + 15 > 100) {
			Health = 100;
		}
		else {
			Health += 15;
		}
	}
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	State->JustUsedBandage = false;
}

void AFortniteCloneCharacter::HoldPickaxe() {
	UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (Animation && State) {
		if (State->CurrentWeapon == 0 && !State->InBuildMode) {
			return; // currently holding a pickaxe while not in build mode
		}
		if (State->HoldingWeapon && State->AimedIn) {
			return; // currently aimed down sight
		}
		if (State->JustUsedBandage || State->JustReloadedRifle || State->JustReloadedShotgun) {
			return; // currently healing or currently reloading
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
			FName WeaponSocketName = TEXT("hand_right_socket");
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);

			FTransform SpawnTransform(GetActorRotation(), GetActorLocation());
			CurrentWeapon = Cast<AWeaponActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, WeaponClasses[0], SpawnTransform));
			CurrentWeaponType = 0;
			if (CurrentWeapon != nullptr)
			{
				//spawnactor has no way of passing parameters so need to use begindeferredactorspawn and finishspawningactor
				CurrentWeapon->Holder = this;

				UGameplayStatics::FinishSpawningActor(CurrentWeapon, SpawnTransform);
			}
			UStaticMeshComponent* WeaponStaticMeshComponent = Cast<UStaticMeshComponent>(CurrentWeapon->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			WeaponStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, WeaponSocketName);

			UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
			if (Animation) {
				Animation->HoldingWeapon = true;
				Animation->AimedIn = false;
				Animation->HoldingWeaponType = 1;
				State->HoldingWeapon = true;
				State->HoldingBandage = false;
				State->CurrentWeapon = 0;
				Animation->HoldingWeaponType = 1;
			}
		}
	}
}

void AFortniteCloneCharacter::HoldAssaultRifle() {
	UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (Animation && State) {
		if (State->CurrentWeapon == 1 && !State->InBuildMode) {
			return; // currently holding a assault rifle while not in build mode
		}
		if (State->HoldingWeapon && State->AimedIn) {
			return; // currently aimed down sight
		}
		if (!State->EquippedWeapons.Contains(1) || State->JustUsedBandage || State->JustReloadedRifle || State->JustReloadedShotgun || State->JustSwungPickaxe) {
			return; // already holding the assault rifle or doesn't have one or is currently healing or currently reloading or swinging pickaxe
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
			FName WeaponSocketName = TEXT("hand_right_socket");
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);

			FTransform SpawnTransform(GetActorRotation(), GetActorLocation());
			CurrentWeapon = Cast<AWeaponActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, WeaponClasses[1], SpawnTransform));
			CurrentWeaponType = 1;
			if (CurrentWeapon != nullptr)
			{
				//spawnactor has no way of passing parameters so need to use begindeferredactorspawn and finishspawningactor
				CurrentWeapon->Holder = this;
				CurrentWeapon->CurrentBulletCount = State->EquippedWeaponsClips[CurrentWeaponType];
				UGameplayStatics::FinishSpawningActor(CurrentWeapon, SpawnTransform);
			}
			UStaticMeshComponent* WeaponStaticMeshComponent = Cast<UStaticMeshComponent>(CurrentWeapon->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			WeaponStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, WeaponSocketName);

			UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
			if (Animation) {
				Animation->HoldingWeapon = true;
				Animation->AimedIn = false;
				Animation->HoldingWeaponType = 1;
				State->HoldingWeapon = true;
				State->HoldingBandage = false;
				State->CurrentWeapon = 1;
				Animation->HoldingWeaponType = 1;
			}
		}
	}
}

void AFortniteCloneCharacter::HoldShotgun() {
	UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (Animation && State) {
		if (State->CurrentWeapon == 2 && !State->InBuildMode) {
			return; // currently holding a shotgun while not in build mode
		}
		if (State->HoldingWeapon && State->AimedIn) {
			return; // currently aimed down sight
		}
		if (!State->EquippedWeapons.Contains(2) || State->JustUsedBandage || State->JustReloadedRifle || State->JustReloadedShotgun ||State->JustSwungPickaxe) {
			return; // already holding the pickaxe or doesn't have one or is currently healing or currently reloading or swinging pickaxe
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
			FName WeaponSocketName = TEXT("hand_right_socket");
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);

			FTransform SpawnTransform(GetActorRotation(), GetActorLocation());
			CurrentWeapon = Cast<AWeaponActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, WeaponClasses[2], SpawnTransform));
			CurrentWeaponType = 2;
			if (CurrentWeapon != nullptr)
			{
				//spawnactor has no way of passing parameters so need to use begindeferredactorspawn and finishspawningactor
				CurrentWeapon->Holder = this;
				CurrentWeapon->CurrentBulletCount = State->EquippedWeaponsClips[CurrentWeaponType];
				UGameplayStatics::FinishSpawningActor(CurrentWeapon, SpawnTransform);
			}
			UStaticMeshComponent* WeaponStaticMeshComponent = Cast<UStaticMeshComponent>(CurrentWeapon->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			WeaponStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, WeaponSocketName);

			UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
			if (Animation) {
				Animation->HoldingWeapon = true;
				Animation->AimedIn = false;
				Animation->HoldingWeaponType = 1;
				State->HoldingWeapon = true;
				State->HoldingBandage = false;
				State->CurrentWeapon = 2;
				Animation->HoldingWeaponType = 1;
			}
		}
	}
}

void AFortniteCloneCharacter::HoldBandage() {
	UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (Animation && State) {
		if (State->JustReloadedRifle || State->JustReloadedShotgun ||State->JustSwungPickaxe) {
			return; //currently reloading weapons or s winging pickaxe
		}
		if (State->HoldingWeapon && State->AimedIn) {
			return; // currently aimed down sight
		}
		if (CurrentWeaponType == -1 && !State->InBuildMode) {
			return; // already holding the bandages while not in build mode
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
			FName BandageSocketName = TEXT("hand_left_socket");
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);

			FTransform SpawnTransform(GetActorRotation(), GetActorLocation());
			CurrentHealingItem = Cast<AHealingActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, BandageClass, SpawnTransform));
			CurrentWeaponType = -1;
			if (CurrentHealingItem != nullptr)
			{
				//spawnactor has no way of passing parameters so need to use begindeferredactorspawn and finishspawningactor
				CurrentHealingItem->Holder = this;

				UGameplayStatics::FinishSpawningActor(CurrentHealingItem, SpawnTransform);
			}
			UStaticMeshComponent* HealingItemStaticMeshComponent = Cast<UStaticMeshComponent>(CurrentHealingItem->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			HealingItemStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, BandageSocketName);

			UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
			if (Animation) {
				Animation->HoldingWeapon = false;
				Animation->AimedIn = false;
				Animation->HoldingWeaponType = 0;
				State->HoldingWeapon = false;
				State->HoldingBandage = true;
				State->CurrentWeapon = -1;
				Animation->HoldingWeaponType = 0;
			}
		}
	}
}

float AFortniteCloneCharacter::GetHealth() {
	return Health;
}

int AFortniteCloneCharacter::GetWoodMaterialCount() {
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	return State->MaterialCounts[0];
}

int AFortniteCloneCharacter::GetStoneMaterialCount() {
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	return State->MaterialCounts[1];
}

int AFortniteCloneCharacter::GetSteelMaterialCount() {
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	return State->MaterialCounts[2];
}

int AFortniteCloneCharacter::GetAssaultRifleAmmoCount() {
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	return State->EquippedWeaponsAmmunition[1] + State->EquippedWeaponsClips[1];
}

int AFortniteCloneCharacter::GetShotgunAmmoCount() {
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	return State->EquippedWeaponsAmmunition[2] + State->EquippedWeaponsClips[2];
}

int AFortniteCloneCharacter::GetBandageCount() {
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	return State->BandageCount;
}

int AFortniteCloneCharacter::GetKillCount() {
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	return State->KillCount;
}