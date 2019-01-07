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

	PlayerInputComponent->BindAction("PickUpItem", IE_Pressed, this, &AFortniteCloneCharacter::PickUpItem);
	PlayerInputComponent->BindAction("Walk", IE_Pressed, this, &AFortniteCloneCharacter::StartWalking);
	PlayerInputComponent->BindAction("Walk", IE_Released, this, &AFortniteCloneCharacter::StopWalking);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFortniteCloneCharacter::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFortniteCloneCharacter::StopSprinting);
	PlayerInputComponent->BindAction("PreviewForwardWall", IE_Pressed, this, &AFortniteCloneCharacter::PreviewForwardWall);
	PlayerInputComponent->BindAction("BuildStructure", IE_Pressed, this, &AFortniteCloneCharacter::BuildStructure);
	PlayerInputComponent->BindAction("ShootGun", IE_Pressed, this, &AFortniteCloneCharacter::ShootGun);

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
	WallPreview = NULL;
}

void AFortniteCloneCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (State) {
		if (State->InBuildMode && State->BuildMode == FString("ForwardWall")) {
			if (WallPreview) {
				WallPreview->Destroy(); //destroy the last wall preview
			}
			WallPreview = GetWorld()->SpawnActor<ABuildingActor>(WallPreviewClass, GetActorLocation() + GetActorForwardVector() * 250, GetActorRotation().Add(0,90,0)); //set the new wall preview
		}
		if (State->HoldingGun) {
			UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
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
}

void AFortniteCloneCharacter::PickUpItem() {
	FHitResult OutHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "e key pressed wut");
	bool SomethingFound = GetWorld()->LineTraceSingleByChannel(OutHit, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 500, ECC_WorldStatic, Params);
	if (SomethingFound) {
		if (OutHit.GetActor()->IsA(AWeaponActor::StaticClass())) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "plz god");
			// PICK UP WEAPON
			FName WeaponSocketName = TEXT("hand_right_socket");
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);

			FRotator OutHitActorRotation = OutHit.GetActor()->GetActorRotation();
			//OutHitActorRotation.Roll += 90.0;
			//OutHitActorRotation.Pitch += 90.0;
			//OutHitActorRotation.Yaw += 90;
			OutHit.GetActor()->SetActorRotation(OutHitActorRotation);

			UStaticMeshComponent* OutHitStaticMeshComponent = Cast<UStaticMeshComponent>(OutHit.GetActor()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			OutHitStaticMeshComponent->AttachToComponent(this->GetMesh(), AttachmentRules, WeaponSocketName);

			AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
			if (State) {
				State->HoldingGun = true;
				UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
				if (Animation) {
					Animation->HoldingGun = true;
				}
			}

		}
		FString text = FString("Found ") + OutHit.GetActor()->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, text);
	}
}

void AFortniteCloneCharacter::StartSprinting() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "shift key pressed");
	GetCharacterMovement()->MaxWalkSpeed = 1200.0;
	UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
	if (Animation) {
		Animation->IsRunning = true;
	}
}

void AFortniteCloneCharacter::StopSprinting() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "shift key released");
	GetCharacterMovement()->MaxWalkSpeed = 400.0;
	UThirdPersonAnimInstance* Animation = Cast<UThirdPersonAnimInstance>(GetMesh()->GetAnimInstance());
	if (Animation) {
		Animation->IsRunning = false;
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

void AFortniteCloneCharacter::PreviewForwardWall() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "x key pressed");
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (State) {
		if (State->BuildMode == FString("ForwardWall")) {
			// getting out of build mode
			State->InBuildMode = false;
			State->BuildMode = FString("None");
			if (WallPreview) {
				WallPreview->Destroy(); //destroy the last wall preview
			}
		}
		else if (State->InBuildMode) {
			// switching to a different build mode
			State->BuildMode = FString("ForwardWall");
		}
		else {
			// getting into build mode
			State->InBuildMode = true;
			State->BuildMode = FString("ForwardWall");
		}
	}
}

void AFortniteCloneCharacter::BuildStructure() {
	AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(GetController()->PlayerState);
	if (State) {
		if (State->InBuildMode && State->BuildMode == FString("ForwardWall")) {
			TArray<AActor*> OverlappingActors;
			ABuildingActor* Wall = GetWorld()->SpawnActor<ABuildingActor>(ForwardWallClass, GetActorLocation() + GetActorForwardVector() * 250, GetActorRotation().Add(0, 90, 0));

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
		if (State->HoldingGun) {
			UAnimInstance* Animation = GetMesh()->GetAnimInstance();
			if (Animation) {
				float Played = PlayAnimMontage(ShootingAnimation);
				FString PlayedString = FString("Played ");
				PlayedString.AppendInt(Played);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, PlayedString);
			}
		}
	}
}