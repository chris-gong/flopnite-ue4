// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileActor.h"
#include "Engine.h"
#include "WeaponActor.h"
#include "FortniteCloneCharacter.h"
#include "BuildingActor.h"

// Sets default values
AProjectileActor::AProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	
	RootComponent = CollisionComp;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = CollisionComp;
	ProjectileMovementComponent->ProjectileGravityScale = 0.01;
	ProjectileMovementComponent->InitialSpeed = 4150000.f;
	ProjectileMovementComponent->MaxSpeed = 4150000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
}

// Called when the game starts or when spawned
void AProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectileActor::OnOverlapBegin);	// set up a notification for when this component hits something blocking
}

// Called every frame
void AProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "projectile tick tick");
}

void AProjectileActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor != NULL) {
		//bullet should only destroy itself once it overlaps with an actor other than itself, the weapon it came from, and the holder of that weapon
		if (OtherActor == (AActor*) Weapon || OtherActor == (AActor*) Weapon->Holder || OtherActor == this) {
			return;
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, OtherActor->GetName());
			if (OtherActor->IsA(AWeaponActor::StaticClass())) {
				//if the weapon has no holder, then let the bullet keep going
				AWeaponActor* WeaponActor = Cast<AWeaponActor>(OtherActor);
				if (WeaponActor->Holder == NULL) {
					return;
				}
				else {
					AFortniteCloneCharacter* FortniteCloneCharacter = Cast<AFortniteCloneCharacter>(WeaponActor->Holder);
					FortniteCloneCharacter->Health -= Damage;
					if (FortniteCloneCharacter->Health <= 0) {
						FortniteCloneCharacter->Destroy();
					}
					Destroy();
				}
			}
			else if (OtherActor->IsA(ABuildingActor::StaticClass())) {
				//make sure the buildingactor is not a preview, if it is a preview then let the bullet keep going
				ABuildingActor* BuildingActor = Cast<ABuildingActor>(OtherActor);
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
			else if (OtherActor->IsA(AFortniteCloneCharacter::StaticClass())) {
				AFortniteCloneCharacter* FortniteCloneCharacter = Cast<AFortniteCloneCharacter>(OtherActor);
				FortniteCloneCharacter->Health -= Damage;
				if (FortniteCloneCharacter->Health <= 0) {
					FortniteCloneCharacter->Destroy();
				}
				Destroy();
			}
			else if (OtherActor->IsA(AProjectileActor::StaticClass())) {
				//let the bullet keep going if it collides with other bullets
				return;
			}
			else {
				Destroy();
			}
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, OtherActor->GetName());
	//GetWorld()->DestroyActor(this);
}