#include "ZSGameMode.h"
#include "Components/ZSHealthComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

AZSGameMode::AZSGameMode()
{
	this->ZombieSpawnTimer = 1.f;
	this->WaitTimer = 2.f;
	this->SpawnTimer = 5.f;
	this->Round = 0;
}

void AZSGameMode::SpawnZombieTimer()
{
	if (NumberOfZombiesToSpawn <= 0)
	{
		EndSpawning();
		return;
	}

	SpawnZombie();

	--NumberOfZombiesToSpawn;
}

void AZSGameMode::StartNextRound()
{
	++Round;

	NumberOfZombiesToSpawn = Round * 2 + 6;

	GetWorldTimerManager().SetTimer(TimerHandle_ZSpawner, this, &AZSGameMode::SpawnZombieTimer, ZombieSpawnTimer, true, SpawnTimer);
}

void AZSGameMode::EndSpawning()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_ZSpawner);
}

void AZSGameMode::StartPlay()
{
	Super::StartPlay();

	StartNextRound();
}

void AZSGameMode::CheckZombies()
{
	if (NumberOfZombiesToSpawn > 0) return;

	bool proceedToNextRound = true;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* pawn = It->Get();
		if (!pawn || pawn->IsPlayerControlled()) continue;

		UZSHealthComponent* healthComponent = Cast<UZSHealthComponent>(pawn->GetComponentByClass(UZSHealthComponent::StaticClass()));
		if (healthComponent && healthComponent->GetCurrentHealth() > 0.f)
		{
			proceedToNextRound = false;
			break;
		}
	}

	if (proceedToNextRound)
	{
		StartNextRound();
	}
}
