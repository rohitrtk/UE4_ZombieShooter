#include "ZSZombie.h"
#include "Components/ZSHealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"

AZSZombie::AZSZombie()
{
	this->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	this->HealthComponent = CreateDefaultSubobject<UZSHealthComponent>(TEXT("Health Component"));
}

void AZSZombie::BeginPlay()
{
	Super::BeginPlay();
	
	this->HealthComponent->OnHealthChanged.AddDynamic(this, &AZSZombie::OnHealthChanged);
}

void AZSZombie::OnHealthChanged(UZSHealthComponent* healthComponent, float health, float healthDelta, const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser)
{
	if (IsDead) return;

	if (health <= 0.f)
	{
		this->IsDead = true;

		this->GetMovementComponent()->StopMovementImmediately();
		this->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		this->GetMesh()->SetSimulatePhysics(true);
		
		//SetLifeSpan(3.f);
	}
}
