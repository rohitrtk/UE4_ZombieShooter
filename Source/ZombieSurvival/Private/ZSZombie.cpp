#include "ZSZombie.h"
#include "Components/ZSHealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "ZSCharacter.h"
#include "ZombieSurvival.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"

AZSZombie::AZSZombie()
{
	this->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	this->HealthComponent = CreateDefaultSubobject<UZSHealthComponent>(TEXT("Health Component"));

	this->AttackRange = 16.f;
	this->AttackDamage = 50.f;
	this->TimeBetweenAttacks = 3.f;

	this->AttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack Box Collider"));
	this->AttackBox->SetRelativeLocation(FVector(53.f, 0.f, 0.f));
	this->AttackBox->SetBoxExtent(FVector(AttackRange, 32.f, 72.f));
	this->AttackBox->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
	this->AttackBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	this->AttackBox->SetupAttachment(RootComponent);
}

void AZSZombie::BeginPlay()
{
	Super::BeginPlay();
	
	this->AttackBox->SetBoxExtent(FVector(AttackRange, 32.f, 72.f));

	this->HealthComponent->OnHealthChanged.AddDynamic(this, &AZSZombie::OnHealthChanged);

	ambientSoundTimer = FMath::RandRange(60, 180);
	if (AmbientSounds.Num() > 0)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_Ambient, this, &AZSZombie::PlayAmbientSound, ambientSoundTimer, false);
	}
}

void AZSZombie::OnHealthChanged(UZSHealthComponent* healthComponent, float health, float healthDelta, const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser)
{
	if (IsDead) return;

	if (health <= 0.f)
	{
		this->IsDead = true;

		this->GetMovementComponent()->StopMovementImmediately();
		this->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		this->GetMesh()->SetSimulatePhysics(true);

		if (DeathSounds.Num() > 0)
		{
			USoundBase* deathSound = DeathSounds[FMath::RandRange(0, DeathSounds.Num() - 1)];
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), deathSound, GetActorLocation());
		}
	}
}

void AZSZombie::BeginAttack()
{
	if (IsAttacking || IsDead) return;

	IsAttacking = true;

	Attack();

	GetWorldTimerManager().SetTimer(TimerHandle_Attack, this, &AZSZombie::EndAttack, TimeBetweenAttacks, false);
}

void AZSZombie::EndAttack()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Attack);

	IsAttacking = false;
}

void AZSZombie::Attack()
{
	TArray<AActor*> overlappingActors;

	AttackBox->GetOverlappingActors(overlappingActors);

	AZSCharacter* potentialPlayer = nullptr;
	for (const auto& actor : overlappingActors)
	{
		potentialPlayer = Cast<AZSCharacter>(actor);
		
		if (!potentialPlayer) continue;
		
		UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
		if (animInstance && AttackAnimMontage)
		{
			UE_LOG(LogTemp, Display, TEXT("Playing animation"));
			animInstance->Montage_Play(AttackAnimMontage);
		}

		UGameplayStatics::ApplyDamage(potentialPlayer, AttackDamage, GetController(), this, UDamageType::StaticClass());
	}

	if (AttackSounds.Num() > 0)
	{
		USoundBase* attackSound = AttackSounds[FMath::RandRange(0, AttackSounds.Num() - 1)];
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), attackSound, GetActorLocation());
	}
}

void AZSZombie::PlayAmbientSound()
{
	if (AmbientSounds.Num() > 0)
	{
		USoundBase* ambientSound = AmbientSounds[FMath::RandRange(0, AmbientSounds.Num() - 1)];
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ambientSound, GetActorLocation());
	}

	ambientSoundTimer = FMath::RandRange(60, 180);
	GetWorldTimerManager().SetTimer(TimerHandle_Ambient, this, &AZSZombie::PlayAmbientSound, ambientSoundTimer, false);
}
