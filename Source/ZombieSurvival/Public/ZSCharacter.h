#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZSCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USpringArmComponent* SpringArmComponent;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;
};
