// Copyright Epic Games, Inc. All Rights Reserved.

#include "FNCharacter.h"
#include "FNPlayerState.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemComponent.h"
#include "FNAbilitySystemComponent.h"
#include "FNAttributeSet.h"
#include "FNCharacterMovementComponent.h"
#include "GameplayAbilities/SprintAbility.h"

//////////////////////////////////////////////////////////////////////////
// AFNCharacter

AFNCharacter::AFNCharacter(const class FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UFNCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))

{
	//UE_LOG(LogTemp, Warning, TEXT("flopnite character construction"));

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	bAlwaysRelevant = false; // TODO: should this be true or false?

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

	ASCInputBound = false;
	JumpEnabled = false;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFNCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFNCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFNCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFNCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFNCharacter::LookUpAtRate);

	BindASCInput();
}

void AFNCharacter::PossessedBy(AController* NewController) {
	// not getting called
	Super::PossessedBy(NewController);

	AFNPlayerState* FNPlayerState = GetPlayerState<AFNPlayerState>();

	if (FNPlayerState) {
		AbilitySystemComponent = Cast<UFNAbilitySystemComponent>(FNPlayerState->GetAbilitySystemComponent());

		if (AbilitySystemComponent) {

			AbilitySystemComponent->InitAbilityActorInfo(FNPlayerState, this);

			AddCharacterAbilities();

			AttributeSet = FNPlayerState->GetAttributeSet();

			InitializeAttributes();
		}
	}
}

void AFNCharacter::OnRep_PlayerState() {
	Super::OnRep_PlayerState();

	AFNPlayerState* FNPlayerState = GetPlayerState<AFNPlayerState>();

	if (FNPlayerState) {
		AbilitySystemComponent = Cast<UFNAbilitySystemComponent>(FNPlayerState->GetAbilitySystemComponent());

		if (AbilitySystemComponent) {
			AbilitySystemComponent->InitAbilityActorInfo(FNPlayerState, this);

			BindASCInput();

			AttributeSet = FNPlayerState->GetAttributeSet();

			InitializeAttributes();
		}
	}
}

UAbilitySystemComponent* AFNCharacter::GetAbilitySystemComponent() const {
	if (AbilitySystemComponent) {
		return Cast<UAbilitySystemComponent>(AbilitySystemComponent);
	}
	else {
		return nullptr;
	}
}

void AFNCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFNCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFNCharacter::MoveForward(float Value)
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

void AFNCharacter::MoveRight(float Value)
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

void AFNCharacter::BindASCInput() {
	if (!ASCInputBound && AbilitySystemComponent && InputComponent)
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), FString("EFNAbilityInputID"), static_cast<int32>(EFNAbilityInputID::Confirm), static_cast<int32>(EFNAbilityInputID::Cancel)));

		ASCInputBound = true;
	}
}

void AFNCharacter::AddCharacterAbilities() {
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	for (auto& Ability : CharacterAbilities) {
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, static_cast<int32>(NewObject<UFNGameplayAbility>(this, Ability->GetSuperClass())->AbilityInputID), this));
	}
	
	AbilitySystemComponent->CharacterAbilitiesGiven = true;
}

void AFNCharacter::InitializeAttributes() {
	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	// TODO: figure out the character level thing
	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
	}
}

float AFNCharacter::GetMoveSpeed() const {
	if (AttributeSet) {
		return AttributeSet->GetMoveSpeed();
	}
	return 0.0f;
}