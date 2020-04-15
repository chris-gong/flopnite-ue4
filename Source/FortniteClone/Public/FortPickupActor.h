// Weekly - open-source on GitHub!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "FortPickupActor.generated.h"

UCLASS()
class FORTNITECLONE_API AFortPickupActor : public AActor
{
	GENERATED_BODY()
	

public:	
	


	AFortPickupActor();

protected:
	
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* MeshComp;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;

	void InInventory(bool In);

};
