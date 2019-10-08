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

AZSZombie::AZSZombie()
{
	this->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	this->HealthComponent = CreateDefaultSubobject<UZSHealthComponent>(TEXT("Health Component"));

	this->AttackRange = 16.f;
	this->AttackDamage = 50.f;

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
	}
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
			animInstance->Montage_Play(AttackAnimMontage);
		}

		UGameplayStatics::ApplyDamage(potentialPlayer, AttackDamage, GetController(), this, UDamageType::StaticClass());
	}
}
