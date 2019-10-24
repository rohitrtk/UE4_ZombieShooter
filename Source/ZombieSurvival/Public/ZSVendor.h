#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZSVendor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UTextRenderComponent;

UCLASS()
class ZOMBIESURVIVAL_API AZSVendor : public AActor
{
	GENERATED_BODY()
	
public:	
	AZSVendor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* InteractCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UTextRenderComponent* TextRenderComponent;

public:	
	virtual void Tick(float DeltaTime) override;

};
