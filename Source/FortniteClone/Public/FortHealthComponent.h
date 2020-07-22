// Weekly - open-source on GitHub!

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FortHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORTNITECLONE_API UFortHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UFortHealthComponent();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Health")
		float Health;

	UPROPERTY(EditDefaultsOnly,  BlueprintReadOnly, Replicated, Category = "Health")
		float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Shield")
		float Shield;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Shield")
		float MaxShield;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float AddHealth(float HealthToAdd);

	UFUNCTION(BlueprintCallable, Category = "Shield")
		float AddShield(float ShieldToAdd);		
};
