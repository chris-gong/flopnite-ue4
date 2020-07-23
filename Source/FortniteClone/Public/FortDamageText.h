// Weekly - open-source on GitHub!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FortDamageText.generated.h"

class UTextRenderComponent;
class USceneComponent;

UCLASS()
class FORTNITECLONE_API AFortDamageText : public AActor
{
	GENERATED_BODY()
	
public:	

	

	AFortDamageText();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent * Text;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USceneComponent * RootComp;

	virtual void Tick(float DeltaTime) override;

};
