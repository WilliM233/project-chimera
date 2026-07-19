// Copyright 2026 Flux Meridian. All Rights Reserved.


#include "ChimeraCharacter.h"

// Sets default values
AChimeraCharacter::AChimeraCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChimeraCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChimeraCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChimeraCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AChimeraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp, Warning, TEXT("[%s] Possessed by %s"), *GetName(), *NewController->GetName());
}

void AChimeraCharacter::UnPossessed()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] UnPossessed"), *GetName());
	Super::UnPossessed();
}

