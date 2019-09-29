#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZSWeapon.generated.h"

class USkeletalMeshComponent;
class UParticleSystem;
class UDamageType;
class USoundBase;

UCLASS()
class ZOMBIESURVIVAL_API AZSWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AZSWeapon();

	void StartFire();
	void StopFire();

	void Reload();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void PlayFireEffects(const FVector& traceEndPoint);

	void Fire();

	void EndReload();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	USoundBase* FireSoundEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	USoundBase* ReloadStartSoundEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	USoundBase* ReloadFinishSoundEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	int32 MaxAmmo;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	int32 CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float DefaultDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float RateOfFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float TimeToReload;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	bool IsReloading;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float BulletSpreadRadius;

private:

	FTimerHandle TimerHandle_TimeBetweenShots;
	FTimerHandle TimerHandle_Reload;

	float lastFireTime;
	float timeBetweenShots;
};
