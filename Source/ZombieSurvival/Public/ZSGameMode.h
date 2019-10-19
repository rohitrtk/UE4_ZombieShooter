#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZSGameMode.generated.h"

#define ZHEALTH			100
#define ZSPEED_SLOW		200
#define ZSPEED_MEDIUM	360
#define ZSPEED_FAST		575

class AZSZombie;
class AZSCharacter;
class UEnvQuery;

struct FEnvQueryResult;

UCLASS()
class ZOMBIESURVIVAL_API AZSGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AZSGameMode();

protected:

	/* Query that determines where the spawn points of the zombies will be */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	UEnvQuery* SpawnQuery;

	/* Function to run when the query executed finishes */
	void OnSpawnQueryFinished(TSharedPtr<FEnvQueryResult> result);

	/* Zombie class that will be spawned */
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

	/* Array of movement speeds for the zombies */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	TArray<int32> ZombieSpeeds;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	bool RoundInProgress;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	AZSCharacter* Player;

	/* Spawns a zombie */
	UFUNCTION()
	void SpawnZombie();

	/* Spawns zombies until the number of zombies for the round has been completed */
	void SpawnZombieTimer();

	/* Starts the next round */
	void StartNextRound();

	/* Stops spawning zombies for the round */
	void EndSpawning();

	/* Checks the current number of zombies in the round */
	void CheckZombies();

	/* Checks if player is ready */
	void CheckPlayersReady();

public:
	virtual void StartPlay() override;
	virtual void Tick(float deltaSeconds) override;

private:
	FTimerHandle TimerHandle_ZSpawner;
};
