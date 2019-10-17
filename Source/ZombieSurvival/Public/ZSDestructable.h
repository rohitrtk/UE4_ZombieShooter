#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZSDestructable.generated.h"

class UStaticMeshComponent;
class UZSHealthComponent;
class USphereComponent;
class USoundBase;

UCLASS()
class ZOMBIESURVIVAL_API AZSDestructable : public AActor
{
	GENERATED_BODY()
	
public:	
	AZSDestructable();

protected:
	virtual void BeginPlay() override;

	/* Destructable static mesh component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* StaticMeshComponent;

	/* Called when destructables health changes */
	UFUNCTION()
	void OnHealthChanged(UZSHealthComponent* healthComponent, float health, float healthDelta, const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser);

	/* Destructables health component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UZSHealthComponent* HealthComponent;

	/* Base damage to deal when destructable is destroyed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float BaseDamage;

	/* Minimum damage to deal when destructable is destroyed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float MinimumDamage;

	/* Inner damage sphere */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	USphereComponent* InnerSphere;

	/* Outer damage sphere */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	USphereComponent* OuterSphere;

	/* Inner damage radius */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float InnerRadius;

	/* Outer damage radius */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float OuterRadius;

	/* Rate of damage fall off */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float DamageFallOff;

	/* Destruction sound effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	USoundBase* DestructionSoundEffect;

	/* Destruction particle effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	UParticleSystem* DestructionParticleEffect;

public:	
	virtual void Tick(float DeltaTime) override;

private:

	/* Is the destructable dead? */
	bool destroyed;
};
