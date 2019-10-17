#include "ZSWeapon.h"
#include "ZombieSurvival.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/ZSScoreComponent.h"
#include "GameFramework/DamageType.h"
#include "TimerManager.h"
#include "ZSCharacter.h"
#include "ZSZombie.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(TEXT("ZS.DebugWeapons"), DebugWeaponDrawing, TEXT("Draws debug lines for weapons"), ECVF_Cheat);

AZSWeapon::AZSWeapon()
{
	this->SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	RootComponent = this->SkeletalMeshComponent;

	this->MuzzleSocketName = "MuzzleSocket";
	this->TracerTargetName = "Target";

	this->DefaultDamage = 15.f;
	this->RateOfFire = 600;

	this->MaxAmmo = 30;
	this->TimeToReload = 2;

	this->BulletSpreadRadius = 2.f;
}

void AZSWeapon::BeginPlay()
{
	Super::BeginPlay();

	this->timeBetweenShots = 60 / RateOfFire;

	this->CurrentAmmo = MaxAmmo;
}

void AZSWeapon::Fire()
{
	AZSCharacter* owner = Cast<AZSCharacter>(GetOwner());
	if (!owner || owner->GetIsReloading() || CurrentAmmo <= 0) return;

	--CurrentAmmo;

	FVector eyePosition;
	FRotator eyeRotation;
	owner->GetActorEyesViewPoint(eyePosition, eyeRotation);

	FVector muzzlePosition = SkeletalMeshComponent->GetSocketLocation(MuzzleSocketName);

	FVector shotDirection = eyeRotation.Vector();

	FVector traceEnd = eyePosition + shotDirection * 10000;
	FVector& traceEndPoint = traceEnd;

	FCollisionQueryParams queryParameters;
	queryParameters.AddIgnoredActor(owner);
	queryParameters.AddIgnoredActor(this);
	queryParameters.bTraceComplex = true;
	queryParameters.bReturnPhysicalMaterial = true;

	AActor* hitActor = nullptr;
	AZSZombie* hitZombie = nullptr;

	FHitResult hitResult;
	if (GetWorld()->LineTraceSingleByChannel(hitResult, eyePosition, traceEnd, COLLISION_WEAPON, queryParameters))
	{
		hitActor = hitResult.GetActor();
		hitZombie = Cast<AZSZombie>(hitActor);

		traceEndPoint = hitResult.ImpactPoint;

		EPhysicalSurface surfaceType = UPhysicalMaterial::DetermineSurfaceType(hitResult.PhysMaterial.Get());

		float damage = DefaultDamage;
		if (surfaceType == SURFACE_FLESHCRITICAL)
		{
			damage *= 3.f;
		}

		UGameplayStatics::ApplyPointDamage(hitActor, damage, shotDirection, hitResult, owner->GetInstigatorController(), this, UDamageType::StaticClass());

		int32 scoreDelta = 0;

		UParticleSystem* selectedEffect = nullptr;
		switch (surfaceType)
		{
		case SURFACE_FLESHDEFAULT:
		case SURFACE_FLESHCRITICAL:
			selectedEffect = FleshImpactEffect;
			
			scoreDelta += 5;

			if (hitZombie && hitZombie->GetIsDead() &&
				surfaceType == SURFACE_FLESHCRITICAL)
			{
				scoreDelta *= 2;
			}

			break;
		default:
			selectedEffect = DefaultImpactEffect;
			break;
		}

		owner->GetScoreComponent()->ChangeScore(scoreDelta);

		if (selectedEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), selectedEffect, hitResult.ImpactPoint, hitResult.ImpactNormal.Rotation());
		}

		lastFireTime = GetWorld()->TimeSeconds;
	}

	if (DebugWeaponDrawing > 0)
	{
		DrawDebugLine(GetWorld(), eyePosition, traceEnd, FColor::Green, false, 1.f, 0, 1.f);
	}

	PlayFireEffects(traceEndPoint);
}

void AZSWeapon::PlayFireEffects(const FVector& traceEndPoint)
{
	if (this->MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, SkeletalMeshComponent, MuzzleSocketName);
	}

	if (this->TracerEffect)
	{
		FVector muzzleLocation = SkeletalMeshComponent->GetSocketLocation(MuzzleSocketName);

		UParticleSystemComponent* tracer = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, muzzleLocation);

		tracer->SetVectorParameter(TracerTargetName, traceEndPoint);
	}

	if (this->FireSoundEffect)
	{
		UGameplayStatics::SpawnSoundAttached(FireSoundEffect, RootComponent);
	}
}

void AZSWeapon::StartFire()
{
	float delay = FMath::Max(lastFireTime + timeBetweenShots - GetWorld()->TimeSeconds, 0.f);

	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &AZSWeapon::Fire, timeBetweenShots, true, delay);
}

void AZSWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void AZSWeapon::Reload()
{
	GetWorldTimerManager().SetTimer(TimerHandle_Reload, this, &AZSWeapon::EndReload, TimeToReload, false);
}

void AZSWeapon::EndReload()
{
	AZSCharacter* owner = Cast<AZSCharacter>(GetOwner());
	owner->SetIsReloading(false);

	this->CurrentAmmo = MaxAmmo;

	GetWorldTimerManager().ClearTimer(TimerHandle_Reload);
}
