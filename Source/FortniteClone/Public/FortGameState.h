// Weekly - open-source on GitHub!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FortGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWinnerFound);

/**
 * 
 */
UCLASS()
class FORTNITECLONE_API AFortGameState : public AGameStateBase
{
	GENERATED_BODY()
	
	AFortGameState();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_Winner, Transient, BlueprintReadOnly, Category = "Game State")
		class AFortniteClonePlayerState* Winner = nullptr;

	UFUNCTION()
		void OnRep_Winner();

	UPROPERTY(BlueprintAssignable, Category = "Game State | Winner")
		FWinnerFound OnWinnerFound;


};
