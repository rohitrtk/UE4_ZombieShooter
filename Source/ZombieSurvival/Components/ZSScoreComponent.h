#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZSScoreComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESURVIVAL_API UZSScoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UZSScoreComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	int32 Score;

public:
	FORCEINLINE void ChangeScore(int32 delta) { this->Score += delta; }
	FORCEINLINE int32 GetScore() const { return this->Score; }
};
