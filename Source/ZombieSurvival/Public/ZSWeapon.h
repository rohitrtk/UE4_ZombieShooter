#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZSWeapon.generated.h"

class USkeletalMeshComponent;
class UParticleSystem;
class USoundBase;
class AZSCharacter;

UCLASS()
class ZOMBIESURVIVAL_API AZSWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AZSWeapon();

	/* Starts firing weapon */
	void StartFire();

	/* Stops firing weapon */
	void StopFire();

	/* Reloads weapon */
	void Reload();

protected:

	virtual void BeginPlay() override;

	/* Plays the fire effects of the weapon */
	UFUNCTION()
	void PlayFireEffects(const FVector& traceEndPoint);

	/* Handles firing logic */
	void Fire();

	/* Stops reloading */
	void EndReload();

	/* Handles moving the player camera to simulate recoil */
	void HandleRecoil(AZSCharacter* owner);

	/* Weapons skeletal mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComponent;

	/* Socket name of the muzzle */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	/* Name of starting location on skeleton for tracer effect */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName;

	/* Particle effect for the muzzle */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	UParticleSystem* MuzzleEffect;

	/* Default particle effect to play upon bullet impact */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	UParticleSystem* DefaultImpactEffect;

	/* Flesh particle effect to play upon bullet impact */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	UParticleSystem* FleshImpactEffect;

	/* Tracer effect for firing */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	UParticleSystem* TracerEffect;

	/* Sound effect to play when weapon is fired */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	USoundBase* FireSoundEffect;

	/* Maximum ammunition of the weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	int32 MaxAmmo;

	/* Current ammunition of the weapon */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	int32 CurrentAmmo;

	/* Default damage of weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float DefaultDamage;

	/* Weapons rate of fire */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float RateOfFire;

	/* Reload time */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float TimeToReload;

	/* Is the gun reloading? */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	bool IsReloading;

	/* Guns bullet spread */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float BulletSpreadRadius;
private:
	FTimerHandle TimerHandle_TimeBetweenShots;
	FTimerHandle TimerHandle_Reload;

	float lastFireTime;
	float timeBetweenShots;
};
