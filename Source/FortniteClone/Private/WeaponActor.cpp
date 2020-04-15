// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponActor.h"
#include "DrawDebugHelpers.h"
#include "FortniteClone.h"
#include "FortDamageText.h"
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

	SetReplicates(true);

	TracerTargetName = "Target";

	ShellEjectAttachPoint = "ShellEject";

	BaseDamage = 20.0f;
}

void AWeaponActor::BeginPlay() {
	Super::BeginPlay();

		CurrentAmmo = 30;	
	
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
	DOREPLIFETIME(AWeaponActor, DamText);
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

		for (int i = 0; i < Bullets; i++)
		{
			if (CanShoot())
			{
				UWorld* const World = GetWorld();
				if (World != NULL)
				{
					

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

					AFortniteCloneCharacter * Player = Cast<AFortniteCloneCharacter>(GetOwner());

					UseAmmo();

					FHitResult Hit;
					if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
					{
						FActorSpawnParameters SpawnParams;
						SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

						AActor * Actor = Hit.GetActor();
						ADamage = FMath::RandRange(BaseDamage, MaxDamge);

						
						if (Player)
						{
							AProjectileActor *  Bullet = World->SpawnActor<AProjectileActor>(BulletClass, EyeLocation, EyeRoatation, SpawnParams);
							if (Bullet != nullptr)
							{
								//spawnactor has no way of passing parameters so need to use begindeferredactorspawn and finishspawningactor
								Bullet->Weapon = this;
								Bullet->WeaponHolder = Player;
								Bullet->SetOwner(Player);
								Bullet->Damage = ADamage;
							}
						}

						//UGameplayStatics::ApplyPointDamage(Actor, ADamage, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamgeType);

						TracerEndPoint = Hit.ImpactPoint;


						if (AFortniteCloneCharacter * HitChar = Cast<AFortniteCloneCharacter>(Actor)) {

							DamText = GetWorld()->SpawnActor<AFortDamageText>(DamageTextClass, TracerEndPoint, GetOwner()->GetActorRotation(), SpawnParams);

							DamText->SetOwner(this);

							if (Player->Shield > 0)
							{
								DamText->Text->SetTextRenderColor(FColor::Blue);
								DamText->Text->TextRenderColor = FColor::Blue;
							}


						}

					}

					DrawDebugLine(GetWorld(), EyeLocation, Hit.ImpactPoint, FColor::Red, false, 1.0f, 0, 1.0f);

					PlayFireEffects(TracerEndPoint);

					if (Role == ROLE_Authority)
					{
						HitScanTrace.TraceTo = TracerEndPoint;
					}

					Player->SpawnWeaponSound(FireSound, GetOwner()->GetActorLocation());
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



