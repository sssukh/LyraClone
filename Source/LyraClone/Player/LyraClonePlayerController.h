#pragma once

#include "ModularPlayerController.h"
#include "LyraClonePlayerController.generated.h"

UCLASS()
class ALyraClonePlayerController : public AModularPlayerController
{
	GENERATED_BODY()
public:
	ALyraClonePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};