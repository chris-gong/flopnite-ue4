// Weekly - open-source on GitHub!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "FortPickupActor.generated.h"

UENUM(BlueprintType)
enum class EFortWeaponType : uint8
{
	WT_Common	UMETA(DisplayName = "Common"),
    WT_Uncommon	UMETA(DisplayName = "Uncommon"),
    WT_Rare	    UMETA(DisplayName = "Rare"),
    WT_Epic	    UMETA(DisplayName = "Epic"),
    WT_Legendary UMETA(DisplayName = "Legendary"),
    WT_Mythic UMETA(DisplayName = "Mythic")
};


USTRUCT(BlueprintType)
struct FWeaponInformation
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	UTexture2D * WeaponImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	EFortWeaponType WeaponType;
};


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	WEAPT_Rifle	UMETA(DisplayName = "Rifle"),
    WEAPT_Shotgun	UMETA(DisplayName = "Shotgun"),
    WEAPT_Pickaxe    UMETA(DisplayName = "Pickaxe")
};


UCLASS()
class FORTNITECLONE_API AFortPickupActor : public AActor
{
	GENERATED_BODY()
	

public:	
	


	AFortPickupActor();

protected:
	
	virtual void BeginPlay() override;

public:


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FWeaponInformation WeaponInfo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EWeaponType WeaponTypeEnum;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* MeshComp;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	int32 index;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInInventory(bool In);

	UFUNCTION()
	void InInventory(bool In);

	UPROPERTY(EditAnywhere,  Replicated)
		FName Socket;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponType")
	int32 WeaponType; // 0 for pickaxe, 1 for assault rifle, 2 for shotgun
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	bool IsWeapon;
	
};
