// Copyright 2026 Flux Meridian. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChimeraPlayerController.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class CHIMERA_API AChimeraPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Chimera|Input")
	TObjectPtr<UInputMappingContext> DefaultContext;
};
