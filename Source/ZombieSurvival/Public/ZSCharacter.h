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

	void ZoomIn();
	void ZoomOut();

	void StartFire();
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void ReloadWeapon();

	UFUNCTION()
	void OnHealthChanged(UZSHealthComponent* healthComponent, float health, float healthDelta, const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UZSHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UZSScoreComponent* ScoreComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	TSubclassOf<AZSWeapon> WeaponClass;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	AZSWeapon* CurrentWeapon;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	bool IsReloading;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	FName WeaponSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float DefaultFOV;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float ZoomFOV;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay", meta = (ClampMin = 0.1, ClampMax = 100))
	float ZoomInterpSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool IsZoomed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool IsDead;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	UAnimMontage* ReloadAnimMontage;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	FORCEINLINE UZSScoreComponent* GetScoreComponent() const { return this->ScoreComponent; }
	FORCEINLINE void SetIsReloading(bool isReloading) { this->IsReloading = isReloading; }
	FORCEINLINE bool GetIsReloading() const { return this->IsReloading; }
};
