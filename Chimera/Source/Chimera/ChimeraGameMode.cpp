// Copyright 2026 Flux Meridian. All Rights Reserved.


#include "ChimeraGameMode.h"
#include "ChimeraPlayerPawn.h"

AChimeraGameMode::AChimeraGameMode()
{
	DefaultPawnClass = AChimeraPlayerPawn::StaticClass();
}

