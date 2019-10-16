#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZSZombie.generated.h"

class UZSHealthComponent;
class UBoxComponent;
class UAnimInstance;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool IsDead;

	UFUNCTION(BlueprintCallable)
	void BeginAttack();

	UFUNCTION()
	void Attack();

	UFUNCTION()
	void EndAttack();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float AttackRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float AttackDamage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	bool IsAttacking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float TimeBetweenAttacks;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Component")
	UBoxComponent* AttackBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	UAnimMontage* AttackAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	TArray<USoundBase*> AmbientSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	TArray<USoundBase*> DeathSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	TArray<USoundBase*> AttackSounds;

	UFUNCTION()
	void PlayAmbientSound();

private:
	FTimerHandle TimerHandle_Ambient;
	FTimerHandle TimerHandle_Attack;

	float ambientSoundTimer;
};
