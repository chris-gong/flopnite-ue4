// Weekly - open-source on GitHub!

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FortInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORTNITECLONE_API UFortInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UFortInventoryComponent();

protected:
	
	virtual void BeginPlay() override;
public:

	UPROPERTY(Replicated, BlueprintReadOnly)
		TArray<class AFortPickupActor*> Items;

	UPROPERTY(Replicated, BlueprintReadOnly)
		TArray<int> Slots;


	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		bool AddItem(class AFortPickupActor * Item);

	UFUNCTION()
		void DropItem(class AFortPickupActor * Item);

	UFUNCTION()
		void DropAllItems();

	UFUNCTION()
		bool HasFreeSlots();
};
