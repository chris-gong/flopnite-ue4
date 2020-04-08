// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponActor.h"
#include "DrawDebugHelpers.h"
#include "FortniteClone.h"
#include "FortniteCloneCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectileActor.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/TextRenderComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Net/UnrealNetwork.h"


DEFINE_LOG_CATEGORY(LogWeaponActor);

AWeaponActor::AWeaponActor() {

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	MeshComp->SetupAttachment(SceneComp);

	SetReplicates(true);

	TracerTargetName = "Target";

	ShellEjectAttachPoint = "ShellEject";

	BaseDamage = 20.0f;
}

void AWeaponActor::BeginPlay() {
	Super::BeginPlay();

		CurrentAmmo = 50000;	
	
}

void AWeaponActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AWeaponActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeaponActor, Holder);
	DOREPLIFETIME_CONDITION(AWeaponActor, HitScanTrace, COND_SkipOwner);
	DOREPLIFETIME(AWeaponActor, CurrentBulletCount);
	DOREPLIFETIME(AWeaponActor, FireAnimation);
	DOREPLIFETIME(AWeaponActor, FireSound);
	DOREPLIFETIME(AWeaponActor, ADamage);
	DOREPLIFETIME(AWeaponActor, BaseDamage);
	DOREPLIFETIME(AWeaponActor, MaxDamge);
}


void AWeaponActor::SimulateWeaponFire() {


}




void AWeaponActor::Fire()
{
	// Trace the world, from pawn eyes to crosshair location

	if (Role < ROLE_Authority)
	{
		ServerFire();
	}

	AActor* MyOwner = GetOwner();
	if (MyOwner) {
		if (CanShoot())
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				UseAmmo();

				FVector EyeLocation;
				FRotator EyeRoatation;
				MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRoatation);

				FVector ShotDirection = EyeRoatation.Vector();

				FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

				FCollisionQueryParams QueryParams;
				QueryParams.AddIgnoredActor(this);
				QueryParams.AddIgnoredActor(MyOwner);
				QueryParams.bTraceComplex = true;

				// Particle "Target" parameter
				FVector TracerEndPoint = TraceEnd;

				FHitResult Hit;
				if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
				{


					FActorSpawnParameters SpawnParams;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


					AActor * Actor = Hit.GetActor();
					ADamage = FMath::RandRange(BaseDamage, MaxDamge);

					AFortniteCloneCharacter * Player = Cast<AFortniteCloneCharacter>(GetOwner());
					if (Player)
					{
						if (Player->GetMesh() != nullptr) {

						}
					}

					UGameplayStatics::ApplyPointDamage(Actor, ADamage, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamgeType);
					
					TracerEndPoint = Hit.ImpactPoint;


					if (AFortniteCloneCharacter * HitChar = Cast<AFortniteCloneCharacter>(Actor)) {
					
						AFortDamageText * DamText = GetWorld()->SpawnActor<AFortDamageText>(DamageTextClass, TracerEndPoint, GetOwner()->GetActorRotation(), SpawnParams);

						DamText->SetOwner(this);

					}

				}

				DrawDebugLine(GetWorld(), EyeLocation, Hit.ImpactPoint, FColor::Red, false, 1.0f, 0, 1.0f);

				PlayFireEffects(TracerEndPoint);

				if (Role == ROLE_Authority)
				{
					HitScanTrace.TraceTo = TracerEndPoint;
				}

				// try and play the sound if specified
				if (FireSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetOwner()->GetActorLocation());
				}

				

			}
			else
			{
				return;
			}
		}
		else 
		{
			return;
		}
		
	}
}

void AWeaponActor::OnRep_HitScanTrace()
{
	// Play cosmetic FX
	PlayFireEffects(HitScanTrace.TraceTo);
	//PlayImpactEffects(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
}


bool AWeaponActor::CanShoot()
{
   if (CurrentAmmo > 0)
   {
	   return true;
   }
   else
   {
	   return false;
   }
}

void AWeaponActor::UseAmmo()
{
	CurrentAmmo--;
}

void AWeaponActor::ServerFire_Implementation()
{
	Fire();
}

bool AWeaponActor::ServerFire_Validate()
{
	return true;
}


void AWeaponActor::PlayFireEffects_Implementation(FVector TraceEnd)
{
	
	if (MuzzleFX) {
		UGameplayStatics::SpawnEmitterAttached(MuzzleFX, MeshComp, MuzzleAttachPoint);
	}

	if (TracerEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleAttachPoint);

		UParticleSystemComponent * TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComp)
		{
			TracerComp->SetVectorParameter(TracerTargetName, TraceEnd);
		}
	}
	if (ShellEjectFX)
	{
		UGameplayStatics::SpawnEmitterAttached(ShellEjectFX, MeshComp, ShellEjectAttachPoint);
	}
	
}

bool AWeaponActor::PlayFireEffects_Validate(FVector TraceEnd)
{
	return true;
}



