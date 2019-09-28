#include "ZSCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"

AZSCharacter::AZSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	this->SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	this->SpringArmComponent->bUsePawnControlRotation = true;
	this->SpringArmComponent->TargetArmLength = 170.f;
	this->SpringArmComponent->AddRelativeLocation(FVector(0.f, 0.f, 70.f));
	this->SpringArmComponent->SetupAttachment(RootComponent);

	this->GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	this->CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	this->CameraComponent->AddRelativeLocation(FVector(0.f, 50.f, 0.f));
	this->CameraComponent->SetupAttachment(this->SpringArmComponent);
}

void AZSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
