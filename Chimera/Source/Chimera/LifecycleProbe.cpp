// Copyright 2026 Flux Meridian. All Rights Reserved.


#include "LifecycleProbe.h"

// Sets default values
ALifecycleProbe::ALifecycleProbe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("[%s] Constructor"), *GetName());
}

// Called when the game starts or when spawned
void ALifecycleProbe::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("[%s] BeginPlay"), *GetName());
	
}

// Called every frame
void ALifecycleProbe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastReport += DeltaTime;
	if (TimeSinceLastReport >= 1.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Still ticking - %f since last report"), *GetName(), TimeSinceLastReport);
		TimeSinceLastReport = 0.0f;
	}

	

}

void ALifecycleProbe::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] EndPlay - reason: %d"), *GetName(), (int32)EndPlayReason);
	Super::EndPlay(EndPlayReason);
}

