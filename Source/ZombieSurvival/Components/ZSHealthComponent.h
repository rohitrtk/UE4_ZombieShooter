#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZSHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UZSHealthComponent*, healthComponent, float, health, float, healthDelta, const class UDamageType*, damageType, class AController*, instigatedBy, AActor*, damageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESURVIVAL_API UZSHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UZSHealthComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health Component")
	float StartingHealth;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Health Component")
	float CurrentHealth;

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* damagedActor, float damage, const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser);

public:
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnHealthChangedSignature OnHealthChanged;

	FORCEINLINE void SetStartingHealth(float startingHealth) { this->StartingHealth = startingHealth; }
	FORCEINLINE float GetStartingHealth() const { return this->StartingHealth; }
	FORCEINLINE void SetStartingHealth(float newHealth) { this->CurrentHealth = newHealth; }
	FORCEINLINE float GetCurrentHealth() const { return this->CurrentHealth; }
	FORCEINLINE void GainHealth(float deltaHealth) { this->CurrentHealth = FMath::Clamp(CurrentHealth + deltaHealth, CurrentHealth, 100.f); }
};
