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

//////////////////////////////////////////////////////////////////////////
// AFortniteCloneCharacter

AFortniteCloneCharacter::AFortniteCloneCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

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

	CurrentWeaponIndex = 0;
	BuildingPreview = NULL;
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

	PlayerInputComponent->BindAction("PickUpItem", IE_Pressed, this, &AFortniteCloneCharacter::PickUpItem);
	PlayerInputComponent->BindAction("Walk", IE_Pressed, this, &AFortniteCloneCharacter::StartWalking);
	PlayerInputComponent->BindAction("Walk", IE_Released, this, &AFortniteCloneCharacter::StopWalking);
	PlayerInputComponent->BindAction("PreviewWall", IE_Pressed, this, &AFortniteCloneCharacter::PreviewWall);
	PlayerInputComponent->BindAction("PreviewRamp", IE_Pressed, this, &AFortniteCloneCharacter::PreviewRamp);
	PlayerInputComponent->BindAction("PreviewFloor", IE_Pressed, this, &AFortniteCloneCharacter::PreviewFloor);
	PlayerInputComponent->BindAction("BuildStructure", IE_Pressed, this, &AFortniteCloneCharacter::BuildStructure);
	PlayerInputComponent->BindAction("ShootGun", IE_Pressed, this, &AFortniteCloneCharacter::ShootGun);
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
	if (WeaponClasses[CurrentWeaponIndex]) {
		FName WeaponSocketName = TEXT("hand_right_socket");
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponActor>(WeaponClasses[CurrentWeaponIndex], GetActorLocation(), GetActorRotation());
		UStaticMeshComponent* WeaponStaticMeshComponent = Cast<UStaticMeshComponent>(CurrentWeapon->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		WeaponStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, WeaponSocketName);
		UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
		AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
		if (Animation && State) {
			Animation->HoldingWeapon = true;
			Animation->AimedIn = false;
			Animation->HoldingWeaponType = 1;
			State->HoldingWeapon = true;
		}
	}
}

void AFortniteCloneCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	GetCapsuleComponent()->SetWorldRotation(GetCameraBoom()->GetComponentRotation());

	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());

	if (State) {
		if (State->InBuildMode && State->BuildMode == FString("Wall")) {
			if (BuildingPreview) {
				BuildingPreview->Destroy(); //destroy the last wall preview
			}
			BuildingPreview = GetWorld()->SpawnActor<ABuildingActor>(WallPreviewClass, GetActorLocation() + GetActorForwardVector() * 250, GetActorRotation().Add(0,90,0)); //set the new wall preview
		}
		if (State->InBuildMode && State->BuildMode == FString("Ramp")) {
			if (BuildingPreview) {
				BuildingPreview->Destroy(); //destroy the last wall preview
			}
			BuildingPreview = GetWorld()->SpawnActor<ABuildingActor>(RampPreviewClass, GetActorLocation() + GetActorForwardVector() * 250, GetActorRotation().Add(0, 90, 0)); //set the new wall preview
		}
		if (State->InBuildMode && State->BuildMode == FString("Floor")) {
			if (BuildingPreview) {
				BuildingPreview->Destroy(); //destroy the last wall preview
			}
			BuildingPreview = GetWorld()->SpawnActor<ABuildingActor>(FloorPreviewClass, GetActorLocation() + GetActorForwardVector() * 250, GetActorRotation().Add(0, 90, 0)); //set the new wall preview
		}
		if (State->HoldingWeapon) {
			if (Animation) {
				FRotator ControlRotation = GetControlRotation();
				FRotator ActorRotation = GetActorRotation();

				FRotator DeltaRotation = ControlRotation - ActorRotation;
				DeltaRotation.Normalize();

				FRotator AimRotation = FRotator(Animation->AimPitch, Animation->AimYaw, 0);
				FRotator InterpolatedRotation = FMath::RInterpTo(AimRotation, DeltaRotation, DeltaTime, Animation->InterpSpeed);
				
				float NewPitch = FMath::ClampAngle(InterpolatedRotation.Pitch, -90, 90);
				float NewYaw = FMath::ClampAngle(InterpolatedRotation.Yaw, -90, 90);
				
				Animation->AimPitch = NewPitch;
				Animation->AimYaw = NewYaw;
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
	if ((Controller != NULL) && (Value != 0.0f))
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
	if ( (Controller != NULL) && (Value != 0.0f) )
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

void AFortniteCloneCharacter::PickUpItem() {
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (State->InBuildMode) {
		return; // can't pick up items while in build mode
	}
	FHitResult OutHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "e key pressed wut");
	bool SomethingFound = GetWorld()->LineTraceSingleByChannel(OutHit, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 500, ECC_WorldStatic, Params);
	if (SomethingFound) {
		if (OutHit.GetActor()->IsA(AWeaponActor::StaticClass())) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "plz god");
			// Destroy old weapon
			CurrentWeapon->Destroy();
			// PICK UP WEAPON
			FName WeaponSocketName = TEXT("hand_right_socket");
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);

			//FRotator OutHitActorRotation = OutHit.GetActor()->GetActorRotation();
			//OutHitActorRotation.Roll += 90.0;
			//OutHitActorRotation.Pitch += 90.0;
			//OutHitActorRotation.Yaw += 90;
			//OutHit.GetActor()->SetActorRotation(OutHitActorRotation);
			CurrentWeapon = Cast<AWeaponActor>(OutHit.GetActor());
			CurrentWeaponIndex = 1;
			UStaticMeshComponent* OutHitStaticMeshComponent = Cast<UStaticMeshComponent>(OutHit.GetActor()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			OutHitStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, WeaponSocketName);

			if (State) {
				State->HoldingWeapon = true;
				State->EquippedWeapons.Add(1); // need to change this for different weapons
				State->CurrentWeapon = 1;
				UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
				if (Animation) {
					Animation->HoldingWeapon = true;
					Animation->HoldingWeaponType = 1;
				}
			}

		}
		FString text = FString("Found ") + OutHit.GetActor()->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, text);
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
			GetCharacterMovement()->MaxWalkSpeed = 400.0;
			Animation->IsRunning = false;
		}
		else {
			// can only sprint if the w key is held down by itself or in combination with the a or d keys
			if (!(OnlyAOrDDown || SDown) && WDown) {
				GetCharacterMovement()->MaxWalkSpeed = 1000.0;
				Animation->IsRunning = true;
			}
			else {
				GetCharacterMovement()->MaxWalkSpeed = 400.0;
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "x key pressed");
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (State) {
		if (State->BuildMode == FString("Wall")) {
			// getting out of build mode
			State->InBuildMode = false;
			State->BuildMode = FString("None");
			if (BuildingPreview) {
				BuildingPreview->Destroy(); //destroy the last wall preview
			}
			// equip weapon being held before
			if (WeaponClasses[CurrentWeaponIndex]) {
				FName WeaponSocketName = TEXT("hand_right_socket");
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
				CurrentWeapon = GetWorld()->SpawnActor<AWeaponActor>(WeaponClasses[CurrentWeaponIndex], GetActorLocation(), GetActorRotation());
				UStaticMeshComponent* WeaponStaticMeshComponent = Cast<UStaticMeshComponent>(CurrentWeapon->GetComponentByClass(UStaticMeshComponent::StaticClass()));
				WeaponStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, WeaponSocketName);
				UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
				if (Animation) {
					Animation->HoldingWeapon = true;
					Animation->AimedIn = false;
					State->HoldingWeapon = true;
					if (State->CurrentWeapon != 0) {
						Animation->HoldingWeaponType = 1;
					}
					else {
						Animation->HoldingWeaponType = 0;
					}
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
			State->AimedIn = false;
			UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
			if (Animation) {
				Animation->HoldingWeapon = false;
				Animation->AimedIn = false;
				Animation->HoldingWeaponType = 0;
			}
			// unequip weapon
			if (CurrentWeapon) {
				CurrentWeapon->Destroy();
			}
		}
	}
}

void AFortniteCloneCharacter::PreviewRamp() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "c key pressed");
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "c key presse2d");
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (State) {
		if (State->BuildMode == FString("Ramp")) {
			// getting out of build mode
			State->InBuildMode = false;
			State->BuildMode = FString("None");
			if (BuildingPreview) {
				BuildingPreview->Destroy(); //destroy the last wall preview
			}
			// equip weapon being held before
			if (WeaponClasses[CurrentWeaponIndex]) {
				FName WeaponSocketName = TEXT("hand_right_socket");
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
				CurrentWeapon = GetWorld()->SpawnActor<AWeaponActor>(WeaponClasses[CurrentWeaponIndex], GetActorLocation(), GetActorRotation());
				UStaticMeshComponent* WeaponStaticMeshComponent = Cast<UStaticMeshComponent>(CurrentWeapon->GetComponentByClass(UStaticMeshComponent::StaticClass()));
				WeaponStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, WeaponSocketName);
				UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
				if (Animation) {
					Animation->HoldingWeapon = true;
					Animation->AimedIn = false;
					State->HoldingWeapon = true;
					if (State->CurrentWeapon != 0) {
						Animation->HoldingWeaponType = 1;
					}
					else {
						Animation->HoldingWeaponType = 0;
					}
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
			State->AimedIn = false;
			UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
			if (Animation) {
				Animation->HoldingWeapon = false;
				Animation->AimedIn = false;
				Animation->HoldingWeaponType = 0;
			}
			// unequip weapon
			if (CurrentWeapon) {
				CurrentWeapon->Destroy();
			}
		}
	}
}

void AFortniteCloneCharacter::PreviewFloor() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "f key pressed");
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (State) {
		if (State->BuildMode == FString("Floor")) {
			// getting out of build mode
			State->InBuildMode = false;
			State->BuildMode = FString("None");
			if (BuildingPreview) {
				BuildingPreview->Destroy(); //destroy the last wall preview
			}
			// equip weapon being held before
			if (WeaponClasses[CurrentWeaponIndex]) {
				FName WeaponSocketName = TEXT("hand_right_socket");
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
				CurrentWeapon = GetWorld()->SpawnActor<AWeaponActor>(WeaponClasses[CurrentWeaponIndex], GetActorLocation(), GetActorRotation());
				UStaticMeshComponent* WeaponStaticMeshComponent = Cast<UStaticMeshComponent>(CurrentWeapon->GetComponentByClass(UStaticMeshComponent::StaticClass()));
				WeaponStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, WeaponSocketName);
				UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
				if (Animation) {
					Animation->HoldingWeapon = true;
					Animation->AimedIn = false;
					Animation->HoldingWeaponType = 1;
					State->HoldingWeapon = true;
					if (State->CurrentWeapon != 0) {
						Animation->HoldingWeaponType = 1;
					}
					else {
						Animation->HoldingWeaponType = 0;
					}
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
			State->AimedIn = false;
			UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
			if (Animation) {
				Animation->HoldingWeapon = false;
				Animation->AimedIn = false;
				Animation->HoldingWeaponType = 0;
			}
			// unequip weapon
			if (CurrentWeapon) {
				CurrentWeapon->Destroy();
			}
		}
	}
}

void AFortniteCloneCharacter::BuildStructure() {
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (State) {
		if (State->InBuildMode && State->BuildMode == FString("Wall")) {
			TArray<AActor*> OverlappingActors;
			ABuildingActor* Wall = GetWorld()->SpawnActor<ABuildingActor>(WallClass, GetActorLocation() + GetActorForwardVector() * 250, GetActorRotation().Add(0, 90, 0));

			Wall->GetOverlappingActors(OverlappingActors);

			for (int i = 0; i < OverlappingActors.Num(); i++) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(OverlappingActors[i]->GetName()));
			}

			if (OverlappingActors.Num() == 0) {
				Wall->Destroy();
			}
		}
		else if (State->InBuildMode && State->BuildMode == FString("Ramp")) {
			TArray<AActor*> OverlappingActors;
			ABuildingActor* Wall = GetWorld()->SpawnActor<ABuildingActor>(RampClass, GetActorLocation() + GetActorForwardVector() * 250, GetActorRotation().Add(0, 90, 0));

			Wall->GetOverlappingActors(OverlappingActors);

			for (int i = 0; i < OverlappingActors.Num(); i++) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(OverlappingActors[i]->GetName()));
			}

			if (OverlappingActors.Num() == 0) {
				Wall->Destroy();
			}
		}
		else if (State->InBuildMode && State->BuildMode == FString("Floor")) {
			TArray<AActor*> OverlappingActors;
			ABuildingActor* Wall = GetWorld()->SpawnActor<ABuildingActor>(FloorClass, GetActorLocation() + GetActorForwardVector() * 250, GetActorRotation().Add(0, 90, 0));

			Wall->GetOverlappingActors(OverlappingActors);

			for (int i = 0; i < OverlappingActors.Num(); i++) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(OverlappingActors[i]->GetName()));
			}

			if (OverlappingActors.Num() == 0) {
				Wall->Destroy();
			}
		}
	}
}

void AFortniteCloneCharacter::ShootGun() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "shoot gun key pressed");
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (State) {
		if (State->HoldingWeapon && State->CurrentWeapon != 0) {
			UAnimInstance* Animation = GetMesh()->GetAnimInstance();
			if (Animation) {
				if (State->AimedIn) {
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Iron sights shot");
					PlayAnimMontage(IronsightsShootingAnimation);
				}
				else {
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "hip shot");
					PlayAnimMontage(HipShootingAnimation);
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
		GetCharacterMovement()->MaxWalkSpeed = 400.0;
		State->AimedIn = false;
	}
}