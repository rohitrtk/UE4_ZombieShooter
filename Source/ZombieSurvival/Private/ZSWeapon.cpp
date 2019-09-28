#include "ZSWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

AZSWeapon::AZSWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	this->SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	RootComponent = this->SkeletalMeshComponent;

	this->MuzzleSocketName = "MuzzleSocket";
	this->TracerTargetName = "Target";
}

void AZSWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZSWeapon::Fire()
{
	AActor* owner = GetOwner();
	if (!owner) return;

	FVector eyePosition;
	FRotator eyeRotation;
	owner->GetActorEyesViewPoint(eyePosition, eyeRotation);

	FVector shotDirection = eyeRotation.Vector();

	FVector traceEnd = eyePosition + shotDirection * 10000;
	FVector& traceEndPoint = traceEnd;

	FCollisionQueryParams queryParameters;
	queryParameters.AddIgnoredActor(owner);
	queryParameters.AddIgnoredActor(this);
	queryParameters.bTraceComplex = true;

	AActor* hitActor = nullptr;

	FHitResult hitResult;
	if (GetWorld()->LineTraceSingleByChannel(hitResult, eyePosition, traceEnd, ECC_Visibility))
	{
		hitActor = hitResult.GetActor();

		traceEndPoint = hitResult.ImpactPoint;

		UGameplayStatics::ApplyPointDamage(hitActor, 20.f, shotDirection, hitResult, owner->GetInstigatorController(), this, DamageType);

		if (this->ImpactEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, hitResult.ImpactPoint, hitResult.ImpactNormal.Rotation());
		}
	}

	DrawDebugLine(GetWorld(), eyePosition, traceEnd, FColor::Green, false, 1.f, 0, 1.f);

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
}
