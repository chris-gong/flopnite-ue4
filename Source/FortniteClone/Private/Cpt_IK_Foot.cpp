// Weekly - open-source on GitHub!


#include "Cpt_IK_Foot.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"


UCpt_IK_Foot::UCpt_IK_Foot()
{
	PrimaryComponentTick.bCanEverTick = true;

	m_fIKFeetInterpSpeed = 22.0f;
	m_fIKHipsInterpSpeed = 17.0f;
	m_fIKTraceDistance = 55.0f;
	m_IKAdjustOffset = 5.0f;

	m_bDebug = false;
}

// Called when the game starts
void UCpt_IK_Foot::BeginPlay()
{
	Super::BeginPlay();

	m_fDeltaTime = 0.0f;

	m_pCharacter = Cast<ACharacter>(GetOwner());
	if (m_pCharacter == nullptr) return;

	m_fIKCapsuleHalkHeight = m_pCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	SetIKActive(true);
}

void UCpt_IK_Foot::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	m_pCharacter = nullptr;
}

// Called every frame
void UCpt_IK_Foot::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (m_pCharacter == nullptr) return;
	if (m_bActive == false) return;

	IK_Update(DeltaTime);
	IK_Debug();
}

void UCpt_IK_Foot::Set_IKSocketName(FString sSocName_LeftFoot, FString sSocName_RightFoot)
{
	m_sIKSocketName_LeftFoot = FName(*sSocName_LeftFoot);
	m_sIKSocketName_RightFoot = FName(*sSocName_RightFoot);
}

void UCpt_IK_Foot::IK_ResetVars()
{
	//! All reset the Animvalues
	IK_Update_FootOffset(m_fDeltaTime, 0.0f, &m_stIKAnimValuse.m_fEffectorLocation_Right, m_fIKFeetInterpSpeed);
	IK_Update_FootOffset(m_fDeltaTime, 0.0f, &m_stIKAnimValuse.m_fEffectorLocation_Left, m_fIKFeetInterpSpeed);

	IK_Update_FootRotation(m_fDeltaTime, FRotator::ZeroRotator, &m_stIKAnimValuse.m_pFootRotation_Right, m_fIKFeetInterpSpeed);
	IK_Update_FootRotation(m_fDeltaTime, FRotator::ZeroRotator, &m_stIKAnimValuse.m_pFootRotation_Left, m_fIKFeetInterpSpeed);

	IK_Update_FootOffset(m_fDeltaTime, 0.0f, &m_stIKAnimValuse.m_fHipOffset, m_fIKHipsInterpSpeed);
	IK_Update_CapsuleHalfHeight(m_fDeltaTime, 0.0f, true);
}

void UCpt_IK_Foot::IK_Update(float fDeltaTime)
{
	m_fDeltaTime = fDeltaTime;

	//! Get Line Trace Info of ground from foot
	ST_IK_TraceInfo pTrace_Left = IK_FootTrace(m_fIKTraceDistance, m_sIKSocketName_LeftFoot);
	ST_IK_TraceInfo pTrace_Right = IK_FootTrace(m_fIKTraceDistance, m_sIKSocketName_RightFoot);

	if (IsMoving() == false)
	{
		//! Update foot rotation from Impact normal
		IK_Update_FootRotation(fDeltaTime, NormalToRotator(pTrace_Left.pImpactLocation), &m_stIKAnimValuse.m_pFootRotation_Left, m_fIKFeetInterpSpeed);
		IK_Update_FootRotation(fDeltaTime, NormalToRotator(pTrace_Right.pImpactLocation), &m_stIKAnimValuse.m_pFootRotation_Right, m_fIKFeetInterpSpeed);
	}
	else
	{
		IK_Update_FootRotation(fDeltaTime, FRotator::ZeroRotator, &m_stIKAnimValuse.m_pFootRotation_Right, m_fIKFeetInterpSpeed);
		IK_Update_FootRotation(fDeltaTime, FRotator::ZeroRotator, &m_stIKAnimValuse.m_pFootRotation_Left, m_fIKFeetInterpSpeed);
	}


	//! Update Hip Offset(Z)
	//! Get Hip(Pelvis)Offset from foot offset and set CapsuleComponent height
	float fHipsOffset = UKismetMathLibrary::Min(pTrace_Left.fOffset, pTrace_Right.fOffset);
	if (fHipsOffset < 0.0f == false) fHipsOffset = 0.0f;
	IK_Update_FootOffset(fDeltaTime, fHipsOffset, &m_stIKAnimValuse.m_fHipOffset, m_fIKHipsInterpSpeed);
	IK_Update_CapsuleHalfHeight(fDeltaTime, fHipsOffset, false);

	//! Update effector locations of foot
	IK_Update_FootOffset(fDeltaTime, pTrace_Left.fOffset - fHipsOffset, &m_stIKAnimValuse.m_fEffectorLocation_Left, m_fIKFeetInterpSpeed);
	IK_Update_FootOffset(fDeltaTime, -1 * (pTrace_Right.fOffset - fHipsOffset), &m_stIKAnimValuse.m_fEffectorLocation_Right, m_fIKFeetInterpSpeed);

	//! When foot is close to trace effector location disable IK
	//bool bLeftFootNearly = UKismetMathLibrary::NearlyEqual_FloatFloat(m_stIKAnimValuse.m_fEffectorLocation_Left, pTrace_Left.fOffset - fHipsOffset, 1.0f);
	//bool bRightFootNearly = UKismetMathLibrary::NearlyEqual_FloatFloat(m_stIKAnimValuse.m_fEffectorLocation_Right, pTrace_Right.fOffset - fHipsOffset, 1.0f);
	//if (bLeftFootNearly == true && bRightFootNearly == true) SetIKActive(false);

}

void UCpt_IK_Foot::IK_Debug()
{
	//! Debug AnimValues at Printstring
	if (m_bDebug == true)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "Foot_L Rotation : " + m_stIKAnimValuse.m_pFootRotation_Left.ToString(), true, false, FLinearColor::Red, 0.0f);
		UKismetSystemLibrary::PrintString(GetWorld(), "Foot_R Rotation : " + m_stIKAnimValuse.m_pFootRotation_Right.ToString(), true, false, FLinearColor::Red, 0.0f);
		UKismetSystemLibrary::PrintString(GetWorld(), "Foot_L EffectorLocation(Z) : " + FString::SanitizeFloat(m_stIKAnimValuse.m_fEffectorLocation_Left), true, false, FLinearColor::Red, 0.0f);
		UKismetSystemLibrary::PrintString(GetWorld(), "Foot_R EffectorLocation(Z) : " + FString::SanitizeFloat(m_stIKAnimValuse.m_fEffectorLocation_Right), true, false, FLinearColor::Red, 0.0f);
		UKismetSystemLibrary::PrintString(GetWorld(), "HipOffset(Z) : " + FString::SanitizeFloat(m_stIKAnimValuse.m_fHipOffset), true, false, FLinearColor::Red, 0.0f);
	}

	//! Debug the show Characters capsule collider
	if (m_pCharacter != nullptr)
	{
		if (m_pCharacter->GetCapsuleComponent()->bHiddenInGame == m_bDebug)
		{
			m_pCharacter->GetCapsuleComponent()->bHiddenInGame = !m_bDebug;
		}
	}
}

void UCpt_IK_Foot::SetIKDebug(bool bActive)
{
	m_bDebug = bActive;
}


void UCpt_IK_Foot::IK_Update_FootOffset(float fDeltaTime, float fTargetValue, float* fEffectorValue, float fInterpSpeed)
{
	//! Set foot Effectlocation value with FInterpTo
	float fInterpValue = UKismetMathLibrary::FInterpTo(*fEffectorValue, fTargetValue, fDeltaTime, fInterpSpeed);
	*fEffectorValue = fInterpValue;
}

void UCpt_IK_Foot::IK_Update_FootRotation(float fDeltaTime, FRotator pTargetValue, FRotator * pFootRotatorValue, float fInterpSpeed)
{
	//! Set Foot Rotation value with FInterpTo
	FRotator pInterpRotator = UKismetMathLibrary::RInterpTo(*pFootRotatorValue, pTargetValue, fDeltaTime, fInterpSpeed);
	*pFootRotatorValue = pInterpRotator;
}

void UCpt_IK_Foot::IK_Update_CapsuleHalfHeight(float fDeltaTime, float fHipsShifts, bool bResetDefault)
{
	UCapsuleComponent* pCapsule = m_pCharacter->GetCapsuleComponent();
	if (pCapsule == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("IK : Capsule is NULL"));
		return;
	}

	//! Get Half Height of capsule component
	float fCapsuleHalf = 0.0f;
	if (bResetDefault == true)
	{
		fCapsuleHalf = m_fIKCapsuleHalkHeight;
	}
	else
	{
		float fHalfAbsSize = UKismetMathLibrary::Abs(fHipsShifts) * 0.5f;
		fCapsuleHalf = m_fIKCapsuleHalkHeight - fHalfAbsSize;
	}

	//! Set capsule component height with FInterpTo 
	float fScaledCapsuleHalfHeight = pCapsule->GetScaledCapsuleHalfHeight();
	float fInterpValue = UKismetMathLibrary::FInterpTo(fScaledCapsuleHalfHeight, fCapsuleHalf, fDeltaTime, m_fIKHipsInterpSpeed);

	pCapsule->SetCapsuleHalfHeight(fInterpValue, true);
}

ST_IK_TraceInfo UCpt_IK_Foot::IK_FootTrace(float fTraceDistance, FName sSocket)
{
	ST_IK_TraceInfo pTraceInfo;

	//! Set Linetraces startpoint and end point
	FVector pSocketLocation = m_pCharacter->GetMesh()->GetSocketLocation(sSocket);
	FVector pLine_Start = FVector(pSocketLocation.X, pSocketLocation.Y, m_pCharacter->GetActorLocation().Z);
	FVector pLine_End = FVector(pSocketLocation.X, pSocketLocation.Y
		, (m_pCharacter->GetActorLocation().Z - m_fIKCapsuleHalkHeight) - fTraceDistance);

	//! Process Line Trace
	FHitResult pHitResult;
	TArray<AActor*> pIgnore;
	pIgnore.Add(GetOwner());

	EDrawDebugTrace::Type eDebug = EDrawDebugTrace::None;
	if (m_bDebug == true) eDebug = EDrawDebugTrace::ForOneFrame;

	bool bResult = UKismetSystemLibrary::LineTraceSingle(GetWorld(), pLine_Start, pLine_End,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), true, pIgnore, eDebug, pHitResult, true);

	//! Set ImpactNormal and Offset from HitResult
	pTraceInfo.pImpactLocation = pHitResult.ImpactNormal;
	if (pHitResult.IsValidBlockingHit() == true)
	{
		float fImpactLegth = (pHitResult.ImpactPoint - pHitResult.TraceEnd).Size();
		pTraceInfo.fOffset = m_IKAdjustOffset + (fImpactLegth - fTraceDistance);
	}
	else
	{
		pTraceInfo.fOffset = 0.0f;
	}

	return pTraceInfo;
}

bool UCpt_IK_Foot::IsMoving()
{
	float fSpeed = m_pCharacter->GetVelocity().Size();
	if (fSpeed > 0.0f) return true;
	return false;
}

void UCpt_IK_Foot::SetIKActive(bool bActive)
{
	m_bActive = bActive;
}

FRotator UCpt_IK_Foot::NormalToRotator(FVector pVector)
{
	float fAtan2_1 = UKismetMathLibrary::DegAtan2(pVector.Y, pVector.Z);
	float fAtan2_2 = UKismetMathLibrary::DegAtan2(pVector.X, pVector.Z);
	fAtan2_2 *= -1.0f;
	FRotator pResult = FRotator(fAtan2_2, 0.0f, fAtan2_1);

	return pResult;
}