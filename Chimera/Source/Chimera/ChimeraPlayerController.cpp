// Copyright 2026 Flux Meridian. All Rights Reserved.


#include "ChimeraPlayerController.h"
#include "EnhancedInputSubsystems.h"

void AChimeraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultContext, 0);
	}
}
