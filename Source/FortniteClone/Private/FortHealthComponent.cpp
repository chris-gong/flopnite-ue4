// Weekly - open-source on GitHub!


#include "FortHealthComponent.h"
#include "Net/UnrealNetwork.h"


UFortHealthComponent::UFortHealthComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	Health = 100.0f;
	Shield = 0.0f;

}

void UFortHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UFortHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UFortHealthComponent, Health);
	DOREPLIFETIME(UFortHealthComponent, Shield);
}

void UFortHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

float UFortHealthComponent::AddHealth(float HealthToAdd)
{

	float NewHealth = Health += HealthToAdd;

	return NewHealth;

}

float UFortHealthComponent::AddShield(float ShieldToAdd)
{
	float NewShield = Shield += ShieldToAdd;

	return NewShield;
}



