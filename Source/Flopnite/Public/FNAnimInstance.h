

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FNAnimInstance.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FLOPNITE_API UFNAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UFNAnimInstance(  );
	
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="Generic" )
	float Speed;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="Generic" )
	bool IsInAir;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="Generic" )
	float Direction;
	
	virtual void NativeUpdateAnimation( float DeltaSeconds ) override;
	
public:
	UFUNCTION( BlueprintCallable )
	void NOTIFY_Jump();

	UFUNCTION( BlueprintCallable )
	void NOTIFY_JogStart();

	UPROPERTY( BlueprintReadWrite, EditAnywhere )
	bool EnableJump;
	
};
