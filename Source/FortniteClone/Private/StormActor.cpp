// Fill out your copyright notice in the Description page of Project Settings.

#include "StormActor.h"
#include "FortniteCloneCharacter.h"
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "FortniteCloneHUD.h"
#include "GameFramework/GameModeBase.h"
#include "FortniteCloneGameMode.h"
#include "Engine/Engine.h"

// Sets default values
AStormActor::AStormActor()
{
	bReplicates = true;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StormMoving = IsShrinking;
	Damage = 1;
	StormAdvanceStageRate = 30.f; //Default every 30 seconds
	StormIncreaseDamageRate = 120.f; //Default every 120 seconds
	IsShrinking = false;
	CanShrink = true;
	InitialSizeScale = GetActorScale3D();
	InitialActorLocation = GetActorLocation();
	ScaleDownRate = 0.999485f;
	ScaleHighThreshold = 1.0f;
	ScaleMidThreshold = 0.7f;
	ScaleLowThreshold = 0.4f;
	ScaleHighModifier = 0.000110f;
	ScaleMidModifier = 0.000320f;
	ScaleLowModifier = 0.000100f;
	Stage = 0; //Stages 0, 1, 2, 3, the circle is not shrinking, stage 4, 5, 6 the circle is shrinking and sets back to 0 afterwards
	FMath::SRandInit(FPlatformTime::Cycles());

	IConsoleVariable* zStormScale = IConsoleManager::Get().RegisterConsoleVariable(TEXT("z.StormScale"),
		2,
		TEXT("AdminCommand: 'z.StormScale (float value)' to live edit storm z scale\n"),
		ECVF_Scalability | ECVF_RenderThreadSafe);

	zStormScale->AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateLambda([&](IConsoleVariable* Var) {
		InitialSizeScale.Z = SizeScale.Z = FCString::Atof(*Var->GetString());
		FString LogMsg = FString::Printf(_T("SizeScale.Z=%.08f"), SizeScale.Z);
		GEngine->AddOnScreenDebugMessage(4, 5.0f, FColor::Yellow, LogMsg);
	}));

	IConsoleVariable* zStormScale2 = IConsoleManager::Get().RegisterConsoleVariable(TEXT("z.StormScaleAfterDecimal"),
		2,
		TEXT("AdminCommand: 'z.StormScaleAfterDecimal (float value)' to live edit storm z scale\n"),
		ECVF_Scalability | ECVF_RenderThreadSafe);

	zStormScale2->AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateLambda([&](IConsoleVariable* Var) {
		SizeScale.Z = InitialSizeScale.Z + (FCString::Atof(*Var->GetString()) / 100.f);
		FString LogMsg = FString::Printf(_T("SizeScale.Z after decimal=%.08f"), SizeScale.Z);
		GEngine->AddOnScreenDebugMessage(4, 5.0f, FColor::Yellow, LogMsg);
	}));

	IConsoleVariable* zStormLocation = IConsoleManager::Get().RegisterConsoleVariable(TEXT("z.StormLocation"),
		2,
		TEXT("AdminCommand: 'z.StormLocation (float value)' to live edit storm z location\n"),
		ECVF_Scalability | ECVF_RenderThreadSafe);

	zStormLocation->AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateLambda([&](IConsoleVariable* Var) {
		InitialActorLocation.Z = Var->GetFloat();
		SetActorLocation(InitialActorLocation);
	}));
}


void AStormActor::InitializeStorm()
{
	if (HasAuthority())
	{
		Damage = 1;
		IsShrinking = false;
		Stage = 0;
		//move the storm to a random location on the map
		int32 X = FMath::RandRange(-7000 + 10000, 60000 - 10000);
		int32 Y = FMath::RandRange(-60000 + 10000, 17000 - 10000);
		SetActorLocation(FVector(X, Y, InitialActorLocation.Z));

		SizeScale = InitialSizeScale;
		SetActorScale3D(InitialSizeScale);

		GetWorldTimerManager().SetTimer(StormSetupTimerHandle, this, &AStormActor::AdvanceStage, StormAdvanceStageRate, true);
		GetWorldTimerManager().SetTimer(StormDamageTimerHandle, this, &AStormActor::ServerSetNewDamage, StormIncreaseDamageRate, true);
	}

	
}


void AStormActor::BeginPlay()
{
	Super::BeginPlay();
	InitializeStorm();
}


void AStormActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StormMoving = IsShrinking;

	if (HasAuthority() && IsShrinking && CanShrink)
	{
		OnRep_StormMoving();

		if (SizeScale.X > 0.0000009f)
		{
			float newScaleXY = (SizeScale.X - (ScaleDownRate * DeltaTime * 0.009f));
			SizeScale = FVector(newScaleXY, newScaleXY, SizeScale.Z);
			SetActorScale3D(SizeScale);

			FString LogMsg = FString::Printf(_T("Using DeltaTime incorporated scaling: Stage=%i SizeScale.XY=%0.8f"), Stage, SizeScale.X);
			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, LogMsg);
		}



		float timeElapsed1 = GetWorldTimerManager().GetTimerElapsed(StormSetupTimerHandle);
		float timeRemaining1 = GetWorldTimerManager().GetTimerRemaining(StormSetupTimerHandle);
		timeElapsed = timeElapsed1;
		timeRemaining = timeRemaining1;
		FString timeMsg1 = FString::Printf(_T("Storm Stage Advance, timer elapsed (seconds): %.02f, time remaining until next stage (seconds): %.02f"), timeElapsed1, timeRemaining1);
		GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Green, timeMsg1);

		if (StormDamageTimerHandle.IsValid())
		{
			float timeElapsed2 = GetWorldTimerManager().GetTimerElapsed(StormDamageTimerHandle);
			float timeRemaining2 = GetWorldTimerManager().GetTimerRemaining(StormDamageTimerHandle);
			FString timeMsg2 = FString::Printf(_T("Storm Damage Increase, timer elapsed (seconds): %.02f, time remaining until damage increased (seconds): %.02f"), timeElapsed2, timeRemaining2);
			GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Green, timeMsg2);
		}
	}

}

void AStormActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AStormActor, Damage);
	DOREPLIFETIME(AStormActor, IsShrinking);
	DOREPLIFETIME(AStormActor, InitialSizeScale);
	DOREPLIFETIME(AStormActor, InitialActorLocation);
	DOREPLIFETIME(AStormActor, SizeScales);
	DOREPLIFETIME(AStormActor, ScaleIndex);
	DOREPLIFETIME(AStormActor, ScaleTotalCount);
	DOREPLIFETIME(AStormActor, StormMoving);
	DOREPLIFETIME(AStormActor, timeElapsed);
	DOREPLIFETIME(AStormActor, timeRemaining);
}

void AStormActor::AdvanceStage_Implementation()
{

	//Advance stages until storm is down to almost nothing (no longer visible) or ScaleIndex is outside of precalculated shrinkdowns,
	//Usually by this point either the game has probably ended or we reinitalize the storm and start again...
	if (GetActorScale3D().X <= 0.0000009f && GetActorScale3D().Y <= 0.0000009f) //|| ScaleIndex >= ScaleTotalCount)
		return InitializeStorm();
	//If 1/10th the scale make this the last stage and keep shrinking until there's a winner / end of game, or the storm re-initializes due to being extremely tiny as above
	if (GetActorScale3D().X <= 0.1f && GetActorScale3D().Y <= 0.1f)
		return;

	IsShrinking = ((++Stage % 2) != 0) ? true : false; //Every odd numbered stage advance the shrinking!
	StormMoving = IsShrinking;
}

FString AStormActor::GetTimeRemainingUtillNextStage() {
	return FString::SanitizeFloat(timeRemaining);
}

FString AStormActor::GetTimeElapsedUtillNextStage() {
	return FString::SanitizeFloat(timeElapsed);
}

bool AStormActor::AdvanceStage_Validate() {
	return true;
}


void AStormActor::OnRep_StormMoving()
{
	OnStormMove.Broadcast();
}

void AStormActor::ServerSetNewDamage_Implementation() {
	Damage++;
}

bool AStormActor::ServerSetNewDamage_Validate() {
	return true;
}

void AStormActor::ServerStartStorm_Implementation() {

	GetWorldTimerManager().SetTimer(StormStateTimerHandle, this, &AStormActor::AdvanceStage, 0.001f, false);
	GetWorldTimerManager().SetTimer(StormDamageTimerHandle, this, &AStormActor::ServerSetNewDamage, StormIncreaseDamageRate + 0.001f, false);
}

bool AStormActor::ServerStartStorm_Validate() {
	return true;
}