#include "ZSCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "ZombieSurvival.h"
#include "ZSWeapon.h"
#include "Components/ZSHealthComponent.h"

AZSCharacter::AZSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	this->SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	this->SpringArmComponent->bUsePawnControlRotation = true;
	this->SpringArmComponent->TargetArmLength = 170.f;
	this->SpringArmComponent->AddRelativeLocation(FVector(0.f, 0.f, 70.f));
	this->SpringArmComponent->SetupAttachment(RootComponent);

	this->GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	this->GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	this->CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	this->CameraComponent->AddRelativeLocation(FVector(0.f, 50.f, 0.f));
	this->CameraComponent->SetupAttachment(this->SpringArmComponent);

	this->HealthComponent = CreateDefaultSubobject<UZSHealthComponent>(TEXT("Health Component"));

	this->DefaultFOV = 90.f;
	this->ZoomFOV = 40.f;
	this->ZoomInterpSpeed = 10.f;

	this->IsZoomed = false;
	this->IsDead = false;
	this->IsReloading = false;

	this->WeaponSocketName = "WeaponSocket";
}

void AZSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	this->CurrentWeapon = GetWorld()->SpawnActor<AZSWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParameters);
	if (CurrentWeapon)
	{
		this->CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
		this->CurrentWeapon->SetOwner(this);
	}

	this->HealthComponent->OnHealthChanged.AddDynamic(this, &AZSCharacter::OnHealthChanged);
}

void AZSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float playerFOV = IsZoomed ? ZoomFOV : DefaultFOV;
	float currentFOV = FMath::FInterpTo(CameraComponent->FieldOfView, playerFOV, DeltaTime, ZoomInterpSpeed);

	this->CameraComponent->SetFieldOfView(currentFOV);
}

void AZSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AZSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AZSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AZSCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AZSCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AZSCharacter::OnJump);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AZSCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AZSCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AZSCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AZSCharacter::StopFire);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AZSCharacter::ReloadWeapon);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AZSCharacter::ZoomIn);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AZSCharacter::ZoomOut);
}

void AZSCharacter::StartFire()
{
	if (CurrentWeapon && !IsReloading)
	{
		CurrentWeapon->StartFire();
	}
}

void AZSCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void AZSCharacter::ReloadWeapon()
{
	UE_LOG(LogTemp, Log, TEXT("RELOAD"));

	if (CurrentWeapon && !IsReloading)
	{
		CurrentWeapon->Reload();

		this->HandleReloadAnimation();

		IsReloading = true;
	}
}

void AZSCharacter::MoveForward(float delta)
{
	AddMovementInput(GetActorForwardVector(), delta);
}
void AZSCharacter::MoveRight(float delta)
{
	AddMovementInput(GetActorRightVector(), delta);
}

void AZSCharacter::BeginCrouch()
{
	Crouch();
}

void AZSCharacter::EndCrouch()
{
	UnCrouch();
}

void AZSCharacter::OnJump()
{
	Jump();
}

FVector AZSCharacter::GetPawnViewLocation() const
{
	if (!this->CameraComponent) return Super::GetPawnViewLocation();

	return this->CameraComponent->GetComponentLocation();
}

void AZSCharacter::ZoomIn()
{
	this->IsZoomed = true;
}

void AZSCharacter::ZoomOut()
{
	this->IsZoomed = false;
}

void AZSCharacter::OnHealthChanged(UZSHealthComponent* healthComponent, float health, float healthDelta,
	const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser)
{
	if (health <= 0.f && !IsDead)
	{
		this->IsDead = true;

		this->GetMovementComponent()->StopMovementImmediately();
		this->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
