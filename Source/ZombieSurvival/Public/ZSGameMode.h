#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZSGameMode.generated.h"

class AZSZombie;
class UEnvQuery;
struct FEnvQueryResult;

UCLASS()
class ZOMBIESURVIVAL_API AZSGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AZSGameMode();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	UEnvQuery* SpawnQuery;

	void OnSpawnQueryFinished(TSharedPtr<FEnvQueryResult> result);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TSubclassOf<AZSZombie> SpawnClass;

	/* Time in seconds between spawning zombies */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	int32 ZombieSpawnTimer;

	/* Time in seconds between the end of a round and the start of next round */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	int32 WaitTimer;

	/* Time in seconds between starting of a new round & spawning the first zombie */
	UPROPERTY()
	int32 SpawnTimer;

	/* Number of zombies to spawn in the round */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	int32 NumberOfZombiesToSpawn;

	/* Current round */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	int32 Round;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TArray<int32> ZombieSpeeds;

	UFUNCTION()
	void SpawnZombie();

	void SpawnZombieTimer();
	void StartNextRound();
	void EndSpawning();
	void CheckZombies();

public:
	virtual void StartPlay() override;

	virtual void Tick(float deltaSeconds) override;

private:
	FTimerHandle TimerHandle_ZSpawner;
};
