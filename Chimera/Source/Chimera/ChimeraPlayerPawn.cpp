// Copyright 2026 Flux Meridian. All Rights Reserved.


#include "ChimeraPlayerPawn.h"
#include "Components/CapsuleComponent.h"
#include "DefaultMovementSet/CharacterMoverComponent.h"
#include "Backends/MoverNetworkPredictionLiaison.h"

// Sets default values
AChimeraPlayerPawn::AChimeraPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->InitCapsuleSize(34.f, 88.f);
	Capsule->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = Capsule;

	Mover = CreateDefaultSubobject<UCharacterMoverComponent>(TEXT("Mover"));
	MoverBackend = CreateDefaultSubobject<UMoverNetworkPredictionLiaisonComponent>(TEXT("MoverBackend"));
}

// Called when the game starts or when spawned
void AChimeraPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChimeraPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChimeraPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

