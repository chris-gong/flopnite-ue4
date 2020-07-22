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
#include "Sound/SoundCue.h"
#include "Net/UnrealNetwork.h"
#include "FortniteClonePlayerState.h"
#include "GameFramework/Actor.h"
#include "FortniteCloneCharacter.h"


DEFINE_LOG_CATEGORY(LogWeaponActor);

AWeaponActor::AWeaponActor() {

	SetReplicates(true);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh3P"));

	bNetUseOwnerRelevancy = true;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;


	TracerTargetName = "Target";

	ShellEjectAttachPoint = "ShellEject";

	BaseDamage = 20.0f;

	
}

void AWeaponActor::BeginPlay() {
	Super::BeginPlay();

		CurrentAmmo = 30;	
		CurrentBulletCount = CurrentAmmo;
		MaxAmmo = 10;

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
	DOREPLIFETIME(AWeaponActor, ADamage);
	DOREPLIFETIME(AWeaponActor, BaseDamage);
	DOREPLIFETIME(AWeaponActor, MaxDamge);
	DOREPLIFETIME_CONDITION(AWeaponActor, CurrentAmmo, COND_OwnerOnly);
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

	UWorld* const World = GetWorld();
	
	AActor* MyOwner = GetOwner();
	if (GetOwner()) {
		if (IsWeapon)
		{
		  for (int i = 0; i < Bullets; i++)
		   {
				if (CanShoot())
				{
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

						CurrentBulletCount--;
						CurrentAmmo--;

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

								if (Player->Shield > 1)
								{
									DamText->Text->SetTextRenderColor(FColor::Blue);
									DamText->Text->TextRenderColor = FColor::Blue;
								}
							}
							PlayWeaponSound(FireSound);

						}

						DrawDebugLine(GetWorld(), EyeLocation, Hit.ImpactPoint, FColor::Red, false, 1.0f, 0, 1.0f);

						PlayWeaponSound(FireSound);

						PlayFireEffects(TracerEndPoint);

						if (Role == ROLE_Authority)
						{
							HitScanTrace.TraceTo = TracerEndPoint;
						}

						//Player->SpawnWeaponSound(FireSound, GetOwner()->GetActorLocation());
					}
					else
					{
						return;
					}
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "can shoot nope");
					return;
				}
		   }
		}
		else
		{
			AFortniteCloneCharacter * Player = Cast<AFortniteCloneCharacter>(MyOwner);

			AFortniteClonePlayerState* State = Cast<AFortniteClonePlayerState>(Player->GetController()->PlayerState);
			if (State)
			{
				if (State->CurrentWeapon == 0) {
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "pickaxe swung");
					if (State->JustSwungPickaxe) {
						return;
					}
					Player->NetMulticastPlayPickaxeSwingAnimation();
					State->JustSwungPickaxe = true;
					FTimerHandle PickaxeTimerHandle;
					GetWorldTimerManager().SetTimer(PickaxeTimerHandle, Player, &AFortniteCloneCharacter::ServerPickaxeTimeOut, 0.403f, false);
				}
			}				
		}
	}else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "owner null");
	}
	
}

void AWeaponActor::Reload()
{
	/* Push the request to server */
	if (Role < ROLE_Authority)
	{
		ServerReload();
	}

	/* If local execute requested or we are running on the server */
	if (MaxAmmo >= 0)
	{
		//bPendingReload = true;

		//float AnimDuration = PlayWeaponAnimation(ReloadAnim);
		//if (AnimDuration <= 0.0f)
		//{
			//AnimDuration = NoAnimReloadDuration;
		//}

		/*
		GetWorldTimerManager().SetTimer(TimerHandle_StopReload, this, &ASWeapon::StopSimulateReload, AnimDuration, false);
		if (Role == ROLE_Authority)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_ReloadWeapon, this, &ASWeapon::ReloadWeapon, FMath::Max(0.1f, AnimDuration - 0.1f), false);
		}

		if (GetOwner() && GetOwner->IsLocallyControlled())
		{
			//PlayWeaponSound(ReloadSound);
		}
		*/
		int32 ClipDelta = FMath::Min(MaxAmmo - CurrentAmmo, CurrentAmmo - CurrentAmmo);

		if (ClipDelta > 0)
		{
			CurrentAmmo += ClipDelta;
		}
	}

}

void AWeaponActor::ServerReload_Implementation()
{
	Reload();
}

bool AWeaponActor::ServerReload_Validate()
{
	return true;
}

void AWeaponActor::OnRep_HitScanTrace()
{
	// Play cosmetic FX
	PlayFireEffects(HitScanTrace.TraceTo);
	//PlayImpactEffects(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
}


UAudioComponent* AWeaponActor::PlayWeaponSound(USoundCue* SoundToPlay)
{
	UAudioComponent *AC = nullptr;
	if (SoundToPlay && GetOwner())
	{
		AC = UGameplayStatics::SpawnSoundAttached(SoundToPlay, GetOwner()->GetRootComponent());
	}
	return AC;
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



