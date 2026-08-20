// Copyright 2026 Flux Meridian. All Rights Reserved.


#include "ChimeraGameMode.h"
#include "ChimeraPlayerPawn.h"
#include "ChimeraPlayerController.h"

AChimeraGameMode::AChimeraGameMode()
{
	DefaultPawnClass = AChimeraPlayerPawn::StaticClass();
	PlayerControllerClass = AChimeraPlayerController::StaticClass();
}

