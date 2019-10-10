#include "ZSGameMode.h"
#include "Components/ZSHealthComponent.h"

AZSGameMode::AZSGameMode()
{
	this->ZombieSpawnTimer = 1.f;
	this->WaitTimer = 2.f;
	this->NumberOfZombiesToSpawn = 8;
	this->Round = 0;
}

void AZSGameMode::SpawnZombieTimer()
{
	SpawnZombie();

	--NumberOfZombiesToSpawn;

	if (NumberOfZombiesToSpawn <= 0) EndRound();
}

void AZSGameMode::StartRound()
{
	++Round;

	NumberOfZombiesToSpawn = Round * 2 + 6;

	GetWorldTimerManager().SetTimer(TimerHandle_ZSpawner, this, &AZSGameMode::SpawnZombieTimer, ZombieSpawnTimer, true, 2.f);
}

void AZSGameMode::EndRound()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_ZSpawner);

	WaitForNextRound();
}

void AZSGameMode::WaitForNextRound()
{
	GetWorldTimerManager().SetTimer(TimerHandle_RoundStart, this, &AZSGameMode::StartRound, WaitTimer, false);
}

void AZSGameMode::StartPlay()
{
	Super::StartPlay();

	WaitForNextRound();
}

void AZSGameMode::CheckZombies()
{
	if (NumberOfZombiesToSpawn > 0) return;

	bool proceedToNextRound = true;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* pawn = It->Get();
		if (!pawn || pawn->IsPlayerControlled()) continue;

		UZSHealthComponent* healthComponent = pawn->GetComponentByClass(UZSHealthComponent::StaticClass());
		if (healthComponent && healthComponent->GetCurrentHealth() > 0.f)
		{
			proceedToNextRound = false;
			break;
		}
	}

	if (proceedToNextRound)
	{
		WaitForNextRound();
	}
}
