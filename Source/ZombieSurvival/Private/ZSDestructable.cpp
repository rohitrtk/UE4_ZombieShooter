#include "ZSDestructable.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ZSHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystem.h"

AZSDestructable::AZSDestructable()
{
	this->StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	
	this->RootComponent = StaticMeshComponent;

	this->HealthComponent = CreateDefaultSubobject<UZSHealthComponent>(TEXT("Health Component"));

	this->BaseDamage	= 50.f;
	this->MinimumDamage = 5.f;
	this->InnerRadius	= 100.f;
	this->OuterRadius	= 600.f;
	this->DamageFallOff = 5.f;
	
	this->OuterSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Outer Sphere"));
	this->OuterSphere->SetSphereRadius(InnerRadius);
	this->OuterSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->OuterSphere->SetupAttachment(RootComponent);

	this->InnerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Inner Sphere"));
	this->InnerSphere->SetSphereRadius(OuterRadius);
	this->InnerSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->InnerSphere->SetupAttachment(RootComponent);

	this->destroyed = false;
}

void AZSDestructable::BeginPlay()
{
	Super::BeginPlay();

	this->HealthComponent->OnHealthChanged.AddDynamic(this, &AZSDestructable::OnHealthChanged);
}

void AZSDestructable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZSDestructable::OnHealthChanged(UZSHealthComponent* healthComponent, float health, float healthDelta, 
	const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser)
{
	if (health <= 0.f && !destroyed)
	{
		destroyed = true;

		UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), BaseDamage, MinimumDamage, GetActorLocation(), 
			InnerRadius, OuterRadius, DamageFallOff, UDamageType::StaticClass(), TArray<AActor*>());

		if (DestructionParticleEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestructionParticleEffect, GetActorLocation());
		}

		if (DestructionSoundEffect)
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DestructionSoundEffect, GetActorLocation());
		}

		Destroy();
	}
}
