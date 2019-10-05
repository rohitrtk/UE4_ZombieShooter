#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZSZombie.generated.h"

class UZSHealthComponent;
class UAIPerceptionComponent;

UCLASS()
class ZOMBIESURVIVAL_API AZSZombie : public ACharacter
{
	GENERATED_BODY()

public:
	AZSZombie();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChanged(UZSHealthComponent* healthComponent, float health, float healthDelta, const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UZSHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UAIPerceptionComponent* PerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool IsDead;
};
