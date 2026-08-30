#pragma once

#include "CoreMinimal.h"
#include "ChimeraTypes.generated.h"

UENUM(BlueprintType)
enum class EChimeraGait : uint8
{
	Walk,
	Jog,
	Run,
	Sprint
};