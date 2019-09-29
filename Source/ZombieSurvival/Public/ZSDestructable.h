#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZSDestructable.generated.h"

class UStaticMeshComponent;
class UZSHealthComponent;
class USphereComponent;

UCLASS()
class ZOMBIESURVIVAL_API AZSDestructable : public AActor
{
	GENERATED_BODY()
	
public:	
	AZSDestructable();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* StaticMeshComponent;

	UFUNCTION()
	void OnHealthChanged(UZSHealthComponent* healthComponent, float health, float healthDelta, const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UZSHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float MinimumDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	USphereComponent* InnerSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	USphereComponent* OuterSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float InnerRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float OuterRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float DamageFallOff;

public:	
	virtual void Tick(float DeltaTime) override;

private:

	bool destroyed;
};
