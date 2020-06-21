// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileActor.h"
#include "WeaponActor.h"
#include "FortniteCloneCharacter.h"
#include "BuildingActor.h"
#include "HealingActor.h"
#include "MaterialActor.h"
#include "FortniteClonePlayerState.h"
#include "FortniteCloneHUD.h"
#include "StormActor.h"
#include "FortniteClonePlayerController.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/Engine.h"
#include "UnrealNetwork.h"
#include "Components/SphereComponent.h"

// Sets default values
AProjectileActor::AProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;*/

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->bEditableWhenInherited = true;
	RootComponent = CollisionComp;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = CollisionComp;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0;
	/*ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
	ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;*/
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
}

// Called when the game starts or when spawned
void AProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "projectile beginplay");
		CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectileActor::OnOverlapBegin);	// set up a notification for when this component hits something blocking
		FTimerHandle LifeTimerHandle;
		GetWorldTimerManager().SetTimer(LifeTimerHandle, this, &AProjectileActor::SelfDestruct, Lifespan, false);
	}
}

// Called every frame
void AProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "projectile tick tick");
}

void AProjectileActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (HasAuthority()) {
		if (OtherActor != nullptr) {
			//bullet should only destroy itself once it overlaps with an actor other than itself, the weapon it came from, and the holder of that weapon
			if ((Weapon && OtherActor == (AActor*)Weapon) || (WeaponHolder && OtherActor == (AActor*)WeaponHolder) || OtherActor == this) {
				return;
			}
			else {
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("NetMode: ") + FString::FromInt(GetNetMode()) + FString(" Collided Actor: ") + OtherActor->GetName());
				if (OtherActor->IsA(AWeaponActor::StaticClass())) {
					//if the weapon has no holder, then let the bullet keep going
					AWeaponActor* WeaponActor = Cast<AWeaponActor>(OtherActor);
					if (WeaponActor) {
						if (WeaponActor->Holder == nullptr) {
							return;
						}
						else {
							AFortniteCloneCharacter* FortniteCloneCharacter = Cast<AFortniteCloneCharacter>(WeaponActor->Holder);
							if (FortniteCloneCharacter) {
								FortniteCloneCharacter->ClientDrawBloodEffect();
								if (FortniteCloneCharacter->Shield > 0) {
									if (FortniteCloneCharacter->Shield - Damage < 0) {
										int LeftoverDamage = Damage - FortniteCloneCharacter->Shield;
										FortniteCloneCharacter->Shield = 0;
										FortniteCloneCharacter->Health -= LeftoverDamage;
									}
									else {
										FortniteCloneCharacter->Shield -= Damage;
									}
								}
								else {
									FortniteCloneCharacter->Health -= Damage;
								}
								if (WeaponHolder) {
									// draw hitmarker
									WeaponHolder->ClientDrawHitMarker();
								}
								if (FortniteCloneCharacter->Health <= 0) {
									if (WeaponActor) {
										WeaponActor->Destroy();
									}
									if (FortniteCloneCharacter) {
										if (FortniteCloneCharacter->BuildingPreview) {
											FortniteCloneCharacter->BuildingPreview->Destroy();
										}
										if (FortniteCloneCharacter->CurrentHealingItem) {
											FortniteCloneCharacter->CurrentHealingItem->Destroy();
										}
										AFortniteClonePlayerController* FortniteClonePlayerController = Cast<AFortniteClonePlayerController>(FortniteCloneCharacter->GetController());
										if (FortniteClonePlayerController) {
											FortniteClonePlayerController->ServerSwitchToSpectatorMode();
										}
										FortniteCloneCharacter->Destroy();
									}
									FortniteCloneCharacter = Cast<AFortniteCloneCharacter>(WeaponHolder);
									if (FortniteCloneCharacter) {
										if (FortniteCloneCharacter->GetController() && FortniteCloneCharacter->GetController()->PlayerState) {
											AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(FortniteCloneCharacter->GetController()->PlayerState);
											if (State) {
												State->KillCount++;
											}
										}
									}
								}
							}
							Destroy();
						}
					}
				}
				else if (OtherActor->IsA(AHealingActor::StaticClass())) {
					//if the healing item has no holder, then let the bullet keep going
					AHealingActor* HealingActor = Cast<AHealingActor>(OtherActor);
					if (HealingActor) {
						if (HealingActor->Holder == nullptr) {
							return;
						}
						else {
							AFortniteCloneCharacter* FortniteCloneCharacter = Cast<AFortniteCloneCharacter>(HealingActor->Holder);
							if (FortniteCloneCharacter) {
								FortniteCloneCharacter->ClientDrawBloodEffect();
								if (FortniteCloneCharacter->Shield > 0) {
									if (FortniteCloneCharacter->Shield - Damage < 0) {
										int LeftoverDamage = Damage - FortniteCloneCharacter->Shield;
										FortniteCloneCharacter->Shield = 0;
										FortniteCloneCharacter->Health -= LeftoverDamage;
									}
									else {
										FortniteCloneCharacter->Shield -= Damage;
									}
								}
								else {
									FortniteCloneCharacter->Health -= Damage;
								}
								if (WeaponHolder) {
									// draw hitmarker
									WeaponHolder->ClientDrawHitMarker();
								}
								if (FortniteCloneCharacter->Health <= 0) {
									if (HealingActor) {
										HealingActor->Destroy();
									}
									if (FortniteCloneCharacter) {
										if (FortniteCloneCharacter->BuildingPreview) {
											FortniteCloneCharacter->BuildingPreview->Destroy();
										}
										if (FortniteCloneCharacter->CurrentWeapon) {
											FortniteCloneCharacter->CurrentHealingItem->Destroy();
										}
										AFortniteClonePlayerController* FortniteClonePlayerController = Cast<AFortniteClonePlayerController>(FortniteCloneCharacter->GetController());
										if (FortniteClonePlayerController) {
											FortniteClonePlayerController->ServerSwitchToSpectatorMode();
										}
										FortniteCloneCharacter->Destroy();
									}
									FortniteCloneCharacter = Cast<AFortniteCloneCharacter>(WeaponHolder);
									if (FortniteCloneCharacter) {
										if (FortniteCloneCharacter->GetController() && FortniteCloneCharacter->GetController()->PlayerState) {
											AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(FortniteCloneCharacter->GetController()->PlayerState);
											if (State) {
												State->KillCount++;
											}
										}
									}
								}
							}
							Destroy();
						}
					}
				}
				else if (OtherActor->IsA(ABuildingActor::StaticClass())) {
					//make sure the buildingactor is not a preview, if it is a preview then let the bullet keep going
					ABuildingActor* BuildingActor = Cast<ABuildingActor>(OtherActor);
					if (BuildingActor) {
						if (BuildingActor->IsPreview) {
							return;
						}
						else {
							BuildingActor->Health -= Damage;
							if (BuildingActor->Health <= 0) {
								BuildingActor->Destroy();
							}
							Destroy();
						}
					}
				}
				else if (OtherActor->IsA(AFortniteCloneCharacter::StaticClass())) {
					AFortniteCloneCharacter* FortniteCloneCharacter = Cast<AFortniteCloneCharacter>(OtherActor);
					if (FortniteCloneCharacter) {
						FortniteCloneCharacter->ClientDrawBloodEffect();
						if (FortniteCloneCharacter->Shield > 0) {
							if (FortniteCloneCharacter->Shield - Damage < 0) {
								int LeftoverDamage = Damage - FortniteCloneCharacter->Shield;
								FortniteCloneCharacter->Shield = 0;
								FortniteCloneCharacter->Health -= LeftoverDamage;
							}
							else {
								FortniteCloneCharacter->Shield -= Damage;
							}
						}
						else {
							FortniteCloneCharacter->Health -= Damage;
						}
						if (WeaponHolder) {
							// draw hitmarker
							WeaponHolder->ClientDrawHitMarker();
						}
						if (FortniteCloneCharacter->Health <= 0) {
							if (FortniteCloneCharacter && FortniteCloneCharacter->CurrentWeapon) {
								FortniteCloneCharacter->CurrentWeapon->Destroy();
							}
							if (FortniteCloneCharacter && FortniteCloneCharacter->CurrentHealingItem) {
								FortniteCloneCharacter->CurrentHealingItem->Destroy();
							}
							if (FortniteCloneCharacter && FortniteCloneCharacter->BuildingPreview) {
								FortniteCloneCharacter->BuildingPreview->Destroy();
							}
							if (FortniteCloneCharacter) {
								AFortniteClonePlayerController* FortniteClonePlayerController = Cast<AFortniteClonePlayerController>(FortniteCloneCharacter->GetController());
								if (FortniteClonePlayerController) {
									FortniteClonePlayerController->ServerSwitchToSpectatorMode();
								}
								FortniteCloneCharacter->Destroy();
							}
							FortniteCloneCharacter = Cast<AFortniteCloneCharacter>(WeaponHolder);
							if (FortniteCloneCharacter) {
								if (FortniteCloneCharacter->GetController() && FortniteCloneCharacter->GetController()->PlayerState) {
									AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(FortniteCloneCharacter->GetController()->PlayerState);
									if (State) {
										State->KillCount++;
									}
								}
							}
						}
						Destroy();
					}
				}
				else if (OtherActor->IsA(AProjectileActor::StaticClass())) {
					//let the bullet keep going if it collides with other bullets
					return;
				}
				else if (OtherActor->IsA(AStormActor::StaticClass())) {
					//let the bullet keep going if it collides with the storm
					return;
				}
				else if (OtherActor->IsA(AMaterialActor::StaticClass())) {
					AMaterialActor* MaterialActor = Cast<AMaterialActor>(OtherActor);
					if (MaterialActor) {
						MaterialActor->Health -= Damage;
						if (ProjectileType == 0) {
							//increase the counts of the owner of the weapon that shot the projectile
							if (WeaponHolder) {
								AFortniteCloneCharacter* FortniteCloneCharacter = Cast<AFortniteCloneCharacter>(WeaponHolder);
								if (FortniteCloneCharacter && FortniteCloneCharacter->GetController() && FortniteCloneCharacter->GetController()->PlayerState) {
									AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(FortniteCloneCharacter->GetController()->PlayerState);
									if (State) {
										State->MaterialCounts[MaterialActor->MaterialType] += MaterialActor->MaterialCount;
									}
								}
							}
						}
						if (MaterialActor->Health <= 0) {
							MaterialActor->Destroy();
						}
						Destroy();
					}
				}
				else {
					Destroy();
				}
			}
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, OtherActor->GetName());
	//GetWorld()->DestroyActor(this);
}

void AProjectileActor::SelfDestruct() {
	Destroy();
}