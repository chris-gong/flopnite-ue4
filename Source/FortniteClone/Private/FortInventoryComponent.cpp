// Weekly - open-source on GitHub!


#include "FortInventoryComponent.h"

#include "FortniteCloneCharacter.h"
#include "FortPickupActor.h"
#include "Components/MeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "FortniteClone.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "FortPickupActor.h"
#include "WeaponActor.h"
#include "GameFramework/Controller.h"
#include "FortniteClonePlayerState.h"
#include "Net/UnrealNetwork.h"


UFortInventoryComponent::UFortInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Slots = 6;
}

void UFortInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UFortInventoryComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps)  const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UFortInventoryComponent, Items);
	DOREPLIFETIME(UFortInventoryComponent, Slots);
	DOREPLIFETIME(UFortInventoryComponent, SelectedItem);
}


void UFortInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


bool UFortInventoryComponent::HasFreeSlots() {

	if (Items.Num() == 6)
	{
		return false;
	}
	else {
		return true;
	}
}


void UFortInventoryComponent::ServerAddItem_Implementation(class AFortPickupActor * Item)
{
	AFortniteCloneCharacter * player = Cast<AFortniteCloneCharacter>(GetOwner());
	
	if (HasFreeSlots())
	{
		SelectedItem = Items.Add(Item);
		Slots--;
		Items[SelectedItem]->InInventory(true, GetOwner());
		//Item->index = Items.Num();
		if(Item->IsA(AWeaponActor::StaticClass()))
		{
			AWeaponActor * weap = Cast<AWeaponActor>(Items[SelectedItem]);
			player->CurrentWeapon = weap;
			player->HoldingWeaponType = Items[SelectedItem]->WeaponType;
			weap->SetOwner(GetOwner());
			Items[SelectedItem]->SetOwner(GetOwner());
			AttachItem(Items[SelectedItem]);
		}
		else{
			Items[SelectedItem]->SetOwner(GetOwner());
			AttachItem(Items[SelectedItem]);
			return;
		}
		AttachItem(Items[SelectedItem]);
		player->WeaponTypeE = Items[SelectedItem]->WeaponTypeEnum;
	}
}

bool UFortInventoryComponent::ServerAddItem_Validate(class AFortPickupActor* Item){ return true; }

void UFortInventoryComponent::AddItem(class AFortPickupActor* Item)
{
	AFortniteCloneCharacter * player = Cast<AFortniteCloneCharacter>(GetOwner());
	
	if (player->Role < ROLE_Authority)
	{
		ServerAddItem(Item);
		SelectedItem = Items.Add(Item);
	}
}

void UFortInventoryComponent::AttachItem(class AFortPickupActor * Item)
{
	AFortniteCloneCharacter * player = Cast<AFortniteCloneCharacter>(GetOwner());
	
	Item->AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Item->Socket);
	Item->SetActorLocationAndRotation(player->GetMesh()->GetSocketLocation(Item->Socket), player->GetMesh()->GetSocketRotation(Item->Socket));
	player->HoldingWeapon = true;
	player->AimedIn = false;
	player->HoldingWeaponType = Item->WeaponType;
	AWeaponActor * weap = Cast<AWeaponActor>(Item);
	player->CurrentWeapon = weap;
}


void UFortInventoryComponent::SelectItem(uint8 index)
{
	Slots--;
	SelectedItem = index;
	if (Items.IsValidIndex(index))
	{
		Items[index]->SetActorHiddenInGame(false);
	}
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
		Item->InInventory(false, GetOwner());
	}
}



