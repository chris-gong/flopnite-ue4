// Weekly - open-source on GitHub!


#include "Vehicle.h"
#include "Net/UnrealNetwork.h"
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
#include "WheeledVehicleMovementComponent4W.h"
#include "Engine/Engine.h"
#include "UnrealNetwork.h"
#include "FortniteCloneCharacter.h"
#include "FortniteClonePlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

static const FName NAME_SteerInput("Steer");
static const FName NAME_ThrottleInput("Throttle");

AVehicle::AVehicle()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	UWheeledVehicleMovementComponent4W * Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	Vehicle4W->MinNormalizedTireLoad = 0.0f;
	Vehicle4W->MinNormalizedTireLoadFiltered = 0.2f;
	Vehicle4W->MaxNormalizedTireLoad = 2.0f;
	Vehicle4W->MaxNormalizedTireLoadFiltered = 2.0f;

	Vehicle4W->MaxEngineRPM = 57000.0f;
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 400.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.0f, 500.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5730.0f, 400.0f);

	Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(40.0f, 0.7f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.6f);

	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;
	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.65f;

	Vehicle4W->TransmissionSetup.bUseGearAutoBox = true;
	Vehicle4W->TransmissionSetup.GearSwitchTime = 0.14f;

}

void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateInAirControl(DeltaTime);
}

void AVehicle::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(NAME_ThrottleInput, this, &AVehicle::ApplyThrottle);
	PlayerInputComponent->BindAxis(NAME_SteerInput, this, &AVehicle::ApplySteering);

	PlayerInputComponent->BindAxis("LookUp", this, &AVehicle::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &AVehicle::Turn);

	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &AVehicle::OnHandbrakePreesed);

	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &AVehicle::OnHandbrakeReleesed);


}

void AVehicle::ApplyThrottle(float val)
{
	GetVehicleMovementComponent()->SetThrottleInput(val);
}

void AVehicle::ApplySteering(float val)
{
	GetVehicleMovementComponent()->SetSteeringInput(val);
}

void AVehicle::LookUp(float val)
{
	if (val != 0.0f)
	{
		AddControllerPitchInput(val);
	}

}

void AVehicle::Turn(float val)
{
	if (val != 0.0f)
	{
		AddControllerYawInput(val);
	}
}

void AVehicle::OnHandbrakePreesed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void AVehicle::OnHandbrakeReleesed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}


void AVehicle::UpdateInAirControl(float DeltaTime)
{

}
