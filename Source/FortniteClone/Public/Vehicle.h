// Weekly - open-source on GitHub!

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "Vehicle.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class FORTNITECLONE_API AVehicle : public AWheeledVehicle
{
	GENERATED_BODY()
	

public:
	AVehicle();

	virtual void Tick(float DeltaTime) override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// End of APawn interface
public:
	void ApplyThrottle(float val);
	void ApplySteering(float val);

	void LookUp(float val);
	void Turn(float val);

	void OnHandbrakePreesed();
	void OnHandbrakeReleesed();

	void UpdateInAirControl(float DeltaTime);


protected:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FollowCamera;

};
