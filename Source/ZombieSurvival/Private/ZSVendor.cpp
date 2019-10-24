#include "ZSVendor.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"

AZSVendor::AZSVendor()
{
	this->StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));

	this->InteractCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Collider"));

	this->TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Render Component"));
}

void AZSVendor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZSVendor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

