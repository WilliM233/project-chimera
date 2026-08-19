// Copyright 2026 Flux Meridian. All Rights Reserved.


#include "ChimeraPlayerPawn.h"

// Sets default values
AChimeraPlayerPawn::AChimeraPlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

