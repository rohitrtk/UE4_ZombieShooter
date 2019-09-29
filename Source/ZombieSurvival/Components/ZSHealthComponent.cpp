#include "ZSHealthComponent.h"

UZSHealthComponent::UZSHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	StartingHealth = 100.f;
}

void UZSHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = StartingHealth;

	AActor* owner = GetOwner();
	if (owner)
	{
		owner->OnTakeAnyDamage.AddDynamic(this, &UZSHealthComponent::HandleTakeAnyDamage);
	}
}

void UZSHealthComponent::HandleTakeAnyDamage(AActor* damagedActor, float damage, const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser)
{
	if (damage <= 0.f) return;

	this->CurrentHealth = FMath::Clamp(CurrentHealth - damage, 0.f, StartingHealth);

	UE_LOG(LogTemp, Log, TEXT("%s Health: %s"), *GetName(), *FString::SanitizeFloat(CurrentHealth));

	this->OnHealthChanged.Broadcast(this, CurrentHealth, damage, damageType, instigatedBy, damageCauser);
}
