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

	FORCEINLINE bool GetIsDead() { return this->IsDead; }

protected:
	virtual void BeginPlay() override;

	/* Called when health is changed */
	UFUNCTION()
	void OnHealthChanged(UZSHealthComponent* healthComponent, float health, float healthDelta, const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser);

	/* Zombies health component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UZSHealthComponent* HealthComponent;

	/* Is the zombie dead? */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool IsDead;

	/* Start attacking */
	UFUNCTION(BlueprintCallable)
	void BeginAttack();

	/* Main attacking logic */
	UFUNCTION()
	void Attack();

	/* Stop attacking */
	UFUNCTION()
	void EndAttack();

	/* Zombie attack range */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float AttackRange;

	/* Zombie attack damage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float AttackDamage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	bool IsAttacking;

	/* Time between zombie attack */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float TimeBetweenAttacks;

	/* Box component for attacks */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Component")
	UBoxComponent* AttackBox;

	/* Animation montage for attacking */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	UAnimMontage* AttackAnimMontage;

	/* Array of sounds to randomly play */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	TArray<USoundBase*> AmbientSounds;

	/* Array of sounds to play when zombie dies */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	TArray<USoundBase*> DeathSounds;

	/* Array of sounds to play when the zombie attacks */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	TArray<USoundBase*> AttackSounds;

	/* Called randomly to play ambient sounds */
	UFUNCTION()
	void PlayAmbientSound();

private:
	FTimerHandle TimerHandle_Ambient;
	FTimerHandle TimerHandle_Attack;

	float ambientSoundTimer;
};
