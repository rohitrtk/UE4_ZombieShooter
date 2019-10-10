#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZSGameMode.generated.h"

UCLASS()
class ZOMBIESURVIVAL_API AZSGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AZSGameMode();

protected:

	/* Time in seconds between spawning zombies */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	int32 ZombieSpawnTimer;

	/* Time in seconds between the end of a round and the start of next round */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	int32 WaitTimer;

	/* Number of zombies to spawn in the round */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	int32 NumberOfZombiesToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	int32 Round;

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnZombie();

	void SpawnZombieTimer();

	void StartRound();
	void EndRound();
	void WaitForNextRound();
	void CheckZombies();

public:
	virtual void StartPlay() override;

private:

	FTimerHandle TimerHandle_ZSpawner;
	FTimerHandle TimerHandle_RoundStart;

};
