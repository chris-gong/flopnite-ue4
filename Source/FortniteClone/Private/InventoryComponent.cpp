// Weekly - open-source on GitHub!


#include "InventoryComponent.h"
#include "WeaponActor.h"
#include "FortniteClone.h"
#include "FortniteCloneCharacter.h"


UInventoryComponent::UInventoryComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	Slots.Add(4); /** + pickaxe*/
	PickAxeSlots = 1;
}


void UInventoryComponent::BeginPlay(){
	Super::BeginPlay();
}



void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

AWeaponActor* UInventoryComponent::GetCurrentEquippedWeapon()
{
	AFortniteCloneCharacter *  Player = Cast<AFortniteCloneCharacter>(GetOwner());
	return Player->CurrentWeapon;
}

TArray<int32> UInventoryComponent::GetSlotsFree()
{
	return Slots;
}

bool UInventoryComponent::HasFreeSlots()
{
	if (Slots.Num() > 0 )
	{
		return true;
	}
	else if (Slots.Num() < 0 )
	{
		return false;
	}
	else if (Slots.Num() < 1)
	{
		return true;
	}
	else if (Slots.Num() < 2)
	{
		return true;
	}
	else if (Slots.Num() < 3)
	{
		return true;
	}
	else if (Slots.Num() < 4)
	{
		return true;
	}
	else if (Slots.Num() == 4)
	{
		return false;
	}
	else
	{
		return true;
	}
}

