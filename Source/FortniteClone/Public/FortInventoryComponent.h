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

	UFUNCTION(BlueprintCallable)
	void AttachItem(class AFortPickupActor * Item);

public:
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
		int SelectedItem;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SelectedItemAttachSocket;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere)
	TArray<class AFortPickupActor*> Items;

	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere)
	int Slots;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAddItem(class AFortPickupActor * Item);
	
	UFUNCTION(BlueprintCallable)
    void AddItem(class AFortPickupActor* Item);
	
	UFUNCTION()
	void DropItem(class AFortPickupActor * Item);

	UFUNCTION()
	void DropAllItems();

	UFUNCTION(BlueprintPure)
	bool HasFreeSlots();

	UFUNCTION(BlueprintCallable)
	void SelectItem(uint8 index);
};
