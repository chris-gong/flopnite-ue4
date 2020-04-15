// Weekly - open-source on GitHub!


#include "FortInventoryComponent.h"
#include "FortPickupActor.h"
#include "Net/UnrealNetwork.h"


UFortInventoryComponent::UFortInventoryComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	
}



void UFortInventoryComponent::BeginPlay()
{
	Super::BeginPlay();


	
}


void UFortInventoryComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps)  const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UFortInventoryComponent, Items);
}


void UFortInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

bool UFortInventoryComponent::AddItem(class AFortPickupActor * Item)
{
	if (HasFreeSlots())
	{
		Items.Add(Item);
		Item->InInventory(true);
		for (AFortPickupActor * Pickup : Items)
		{
			UE_LOG(LogTemp, Warning, TEXT("Item : %s"), *Pickup->GetName());
		}
		return false;
	}
	
	return false;
}

void UFortInventoryComponent::DropAllItems()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		for (AFortPickupActor * Pickup : Items)
		{
			DropItem(Pickup);
		}

	}
}

void UFortInventoryComponent::DropItem(class AFortPickupActor * Item)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		FVector DropLocation = GetOwner()->GetActorLocation();
		DropLocation.X += FMath::RandRange(-50.0f, 100.0f);
		DropLocation.Y += FMath::RandRange(-50.0f, 100.0f);

		Item->SetActorLocation(DropLocation);
		Item->InInventory(false);
	}
}


bool UFortInventoryComponent::HasFreeSlots() {

	if (Items.Num() == 4)
	{
		return false;
	}
	else {
		return true;
	}
}

