#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZSCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;
class UZSHealthComponent;
class UZSScoreComponent;
class AZSWeapon;

UCLASS()
class ZOMBIESURVIVAL_API AZSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AZSCharacter();

protected:
	virtual void BeginPlay() override;

	void MoveForward(float delta);
	void MoveRight(float delta);

	void BeginCrouch();
	void EndCrouch();

	void OnJump();

	/* Decreases FOV */
	void ZoomIn();

	/* Increases FOV */
	void ZoomOut();

	/* Starts firing weapon */
	void StartFire();

	/* Stops firing weapon */
	void StopFire();

	/* Reloads weapon */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void ReloadWeapon();

	/* Called when health is changed */
	UFUNCTION()
	void OnHealthChanged(UZSHealthComponent* healthComponent, float health, float healthDelta, const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser);

	/* Players camera component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UCameraComponent* CameraComponent;

	/* Spring arm component attached to player & camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USpringArmComponent* SpringArmComponent;

	/* Players health component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UZSHealthComponent* HealthComponent;

	/* Players score component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UZSScoreComponent* ScoreComponent;

	/* Type of weapon player has */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	TSubclassOf<AZSWeapon> WeaponClass;
	
	/* Players current weapon */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	AZSWeapon* CurrentWeapon;

	/* Is the player reloading? */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	bool IsReloading;

	/* Name of players bone socket to spawn weapon at */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	FName WeaponSocketName;

	/* The default FOV player has */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float DefaultFOV;

	/* The ADS FOV player has */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float ZoomFOV;

	/* Speed of interpolation between normal FOV & ADS FOV */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay", meta = (ClampMin = 0.1, ClampMax = 100))
	float ZoomInterpSpeed;

	/* Is the player aiming down sight? */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool IsZoomed;

	/* Is the player dead? */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool IsDead;

	/* Animation montage for reloads */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	UAnimMontage* ReloadAnimMontage;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Return the camera location if the camera exists */
	virtual FVector GetPawnViewLocation() const override;

	FORCEINLINE UZSScoreComponent* GetScoreComponent() const { return this->ScoreComponent; }
	FORCEINLINE void SetIsReloading(bool isReloading) { this->IsReloading = isReloading; }
	FORCEINLINE bool GetIsReloading() const { return this->IsReloading; }
};
