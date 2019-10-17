#include "ZSGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ZSHealthComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "ZSZombie.h"

AZSGameMode::AZSGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.f;

	ZombieSpeeds.Add(ZSPEED_SLOW);
	ZombieSpeeds.Add(ZSPEED_MEDIUM);
	ZombieSpeeds.Add(ZSPEED_FAST);

	this->ZombieSpawnTimer = 1.f;
	this->WaitTimer = 2.f;
	this->SpawnTimer = 5.f;
	this->Round = 0;
}

void AZSGameMode::SpawnZombie()
{
	FEnvQueryRequest queryRequest = FEnvQueryRequest(SpawnQuery, this);
	queryRequest.Execute(EEnvQueryRunMode::RandomBest25Pct, this, &AZSGameMode::OnSpawnQueryFinished);
}

void AZSGameMode::OnSpawnQueryFinished(TSharedPtr<FEnvQueryResult> result)
{
	if (!result->IsSuccsessful()) return;

	TArray<FVector> locations;
	result->GetAllAsLocations(locations);

	AZSZombie* zombie = GetWorld()->SpawnActor<AZSZombie>(SpawnClass, locations[0], FRotator::ZeroRotator);

	if (!zombie)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn zombie!"));
	}

	float rand = FMath::FRandRange(1, 10);
	int32 zWalkSpeed = 0;

	if (rand < 2.5)		zWalkSpeed = ZombieSpeeds[0];
	else if (rand < 7)	zWalkSpeed = ZombieSpeeds[1];
	else				zWalkSpeed = ZombieSpeeds[2];

	zombie->GetCharacterMovement()->MaxWalkSpeed = zWalkSpeed;
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

	TArray<AZSZombie*> zombies;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		AZSZombie* zombie = Cast<AZSZombie>(It->Get());
		if (!zombie || zombie->IsPlayerControlled()) continue;

		UZSHealthComponent* healthComponent = Cast<UZSHealthComponent>(zombie->GetComponentByClass(UZSHealthComponent::StaticClass()));
		if (healthComponent && healthComponent->GetCurrentHealth() > 0.f)
		{
			proceedToNextRound = false;
			break;
		}

		zombies.Add(zombie);
	}

	if (proceedToNextRound)
	{
		for (const auto& zombie : zombies)
		{
			zombie->Destroy();
		}
		UE_LOG(LogTemp, Display, TEXT("Starting next round"));
		StartNextRound();
	}
}

void AZSGameMode::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	CheckZombies();
}
