// Weekly - open-source on GitHub!


#include "FortGameState.h"
#include "Net/UnrealNetwork.h"

AFortGameState::AFortGameState()
{

}

void AFortGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFortGameState, Winner);
}


void AFortGameState::OnRep_Winner()
{
	OnWinnerFound.Broadcast();
}


