// Fill out your copyright notice in the Description page of Project Settings.

#include "FortniteCloneSpectator.h"
#include "FortniteCloneCharacter.h"
#include "Engine/Engine.h"

// Sets default values
AFortniteCloneSpectator::AFortniteCloneSpectator()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//GetCharacterMovement()->bForceMaxAccel = true;

	BaseTurnRate = 45.f;
}

// Called when the game starts or when spawned
void AFortniteCloneSpectator::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Spectator Begin play"));
	}
}

// Called every frame
void AFortniteCloneSpectator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//FString LogMsg = FString("spectator tick");
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Spectator tick"));
	//UE_LOG(LogMyGame, Warning, TEXT("%s"), *LogMsg);
}

// Called to bind functionality to input
void AFortniteCloneSpectator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFortniteCloneSpectator::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFortniteCloneSpectator::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &AFortniteCloneSpectator::MoveUp);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFortniteCloneSpectator::TurnAtRate);
}

void AFortniteCloneSpectator::MoveForward(float Value) {
	//FString LogMsg = FString("spectator move forward");
	//UE_LOG(LogMyGame, Warning, TEXT("%s"), *LogMsg);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Move forward ") + FString::SanitizeFloat(Value));
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		//FVector CurrentLocation = RootComponent->GetComponentLocation();
		//RootComponent->SetWorldLocation(FVector(CurrentLocation.X + Value, CurrentLocation.Y, CurrentLocation.Z));
	}
}

void AFortniteCloneSpectator::MoveRight(float Value) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Move right ") + FString::SanitizeFloat(Value));
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
		//FVector CurrentLocation = RootComponent->GetComponentLocation();
		//RootComponent->SetWorldLocation(FVector(CurrentLocation.X, CurrentLocation.Y + Value, CurrentLocation.Z));
	}
}

void AFortniteCloneSpectator::MoveUp(float Value) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Move up ") + FString::SanitizeFloat(Value));
	APlayerController* LocalController = Cast<APlayerController>(GetController());
	bool IsShiftDown = LocalController->IsInputKeyDown(EKeys::LeftShift);

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
		if (IsShiftDown) {
			AddMovementInput(Direction, -1 * Value);
		}
		else {
			AddMovementInput(Direction, Value);
		}
		/*FVector CurrentLocation = RootComponent->GetComponentLocation();
		if (IsShiftDown) {
			RootComponent->SetWorldLocation(FVector(CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z + (Value * -1)));
		}*/
	}

}

void AFortniteCloneSpectator::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}


