// Weekly - open-source on GitHub!


#include "FortPickupActor.h"
#include "DrawDebugHelpers.h"
#include "FortniteClone.h"
#include "FortDamageText.h"
#include "FortniteCloneCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectileActor.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/TextRenderComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Net/UnrealNetwork.h"
#include "UnrealNetwork.h"

// Sets default values
AFortPickupActor::AFortPickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	SetReplicates(true);
	bReplicates = true;
	bReplicateMovement = true;

}



void AFortPickupActor::BeginPlay()
{
	Super::BeginPlay();
	
}


void AFortPickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AFortPickupActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps)  const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


void AFortPickupActor::InInventory(bool In)
{
	if (Role == ROLE_Authority)
	{
	}
}
