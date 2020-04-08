// Weekly - open-source on GitHub!

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


class AWeaponActor;
class UItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORTNITECLONE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		AWeaponActor* GetCurrentEquippedWeapon();

	/** Max Slots*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<int32> Slots;

	/** Items The Player is holding 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<UItem> Items;
		*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 PickAxeSlots;

	UFUNCTION()
		TArray<int32> GetSlotsFree();

	UFUNCTION()
		bool HasFreeSlots();
		
};
