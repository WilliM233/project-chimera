// Copyright 2026 Flux Meridian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LifecycleProbe.generated.h"

UCLASS()
class CHIMERA_API ALifecycleProbe : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALifecycleProbe();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	float TimeSinceLastReport = 0.0f;
};
