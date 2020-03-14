// Fill out your copyright notice in the Description page of Project Settings.

#include "ThirdPersonAnimInstance.h"
#include "FortniteCloneCharacter.h"
#include "UnrealNetwork.h"
#include "Engine/Engine.h"

// Add default functionality here for any IGuyAnimInstance functions that are not pure virtual.

UThirdPersonAnimInstance::UThirdPersonAnimInstance()
{
	IsWalking = false;
	IsRunning = false;
	HoldingWeapon = false;
	AimedIn = false;
	HoldingWeaponType = 0;
	Speed = 0.0;
	AimPitch = 0.0;
	AimYaw = 0.0;
	InterpSpeed = 15.0;
	WalkingX = 0;
	WalkingY = 0;
	RunningX = 0;
	RunningY = 0;
};

void UThirdPersonAnimInstance::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UThirdPersonAnimInstance, IsRunning);
	DOREPLIFETIME(UThirdPersonAnimInstance, IsWalking);
	DOREPLIFETIME(UThirdPersonAnimInstance, HoldingWeapon);
	DOREPLIFETIME(UThirdPersonAnimInstance, AimedIn);
	DOREPLIFETIME(UThirdPersonAnimInstance, HoldingWeaponType);
	DOREPLIFETIME(UThirdPersonAnimInstance, Speed);
	DOREPLIFETIME(UThirdPersonAnimInstance, AimPitch);
	DOREPLIFETIME(UThirdPersonAnimInstance, AimYaw);
	DOREPLIFETIME(UThirdPersonAnimInstance, InterpSpeed);
	DOREPLIFETIME(UThirdPersonAnimInstance, WalkingX);
	DOREPLIFETIME(UThirdPersonAnimInstance, WalkingY);
	DOREPLIFETIME(UThirdPersonAnimInstance, RunningX);
	DOREPLIFETIME(UThirdPersonAnimInstance, RunningY);
}

void UThirdPersonAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);
	Tick_IKFoot();
	AFortniteCloneCharacter* FortniteCloneCharacter = Cast<AFortniteCloneCharacter>(TryGetPawnOwner());
	if (FortniteCloneCharacter) {
		IsWalking = FortniteCloneCharacter->IsWalking;
		IsRunning = FortniteCloneCharacter->IsRunning;
		WalkingX = FortniteCloneCharacter->WalkingX;
		WalkingY = FortniteCloneCharacter->WalkingY;
		RunningX = FortniteCloneCharacter->RunningX;
		RunningY = FortniteCloneCharacter->RunningY;
		HoldingWeapon = FortniteCloneCharacter->HoldingWeapon;
		HoldingWeaponType = FortniteCloneCharacter->HoldingWeaponType;
		AimedIn = FortniteCloneCharacter->AimedIn;
		AimPitch = FortniteCloneCharacter->AimPitch;
		AimYaw = FortniteCloneCharacter->AimYaw;
	}
}

void UThirdPersonAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Init_IKFootRef();
}

void UThirdPersonAnimInstance::Init_IKFootRef()
{
	// Get IKFoot Component from owner
	APawn* pOwner = TryGetPawnOwner();
	if (pOwner != nullptr)
	{
		UActorComponent* pActorComp = pOwner->GetComponentByClass(UCpt_IK_Foot::StaticClass());
		if (pActorComp != nullptr)
		{
			m_pIK_Foot_Ref = Cast<UCpt_IK_Foot>(pActorComp);
			if (m_pIK_Foot_Ref == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("IKFootRef is nullptr"));
			}
		}
	}
}

void UThirdPersonAnimInstance::Tick_IKFoot()
{
	if (m_pIK_Foot_Ref == nullptr) return;

	m_stIKAnimValue = m_pIK_Foot_Ref->GetIKAnimValue();
}