

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FNAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FLOPNITE_API UFNAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UFNAnimInstance(  );
	
public:
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category="Generic" )
	float Speed;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category="Generic" )
	bool IsInAir;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category="Generic" )
	float Direction;
	
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category="Generic" )
	float Yaw;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category="Generic" )
	float Pitch;
	
	virtual void NativeUpdateAnimation( float DeltaSeconds ) override;
	
public:
	UFUNCTION( BlueprintCallable )
	void NOTIFY_Jump();

	UFUNCTION( BlueprintCallable )
	void NOTIFY_JogStart();

	UPROPERTY( BlueprintReadOnly, EditAnywhere )
	bool JumpEnabled;

	UPROPERTY( BlueprintReadOnly, EditAnywhere )
	bool CrouchEnabled;
	
};
