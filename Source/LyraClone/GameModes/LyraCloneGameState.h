#pragma once

#include "GameFramework/GameStateBase.h"
#include "LyraCloneGameState.generated.h"

UCLASS()
class ALyraCloneGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ALyraCloneGameState(const FObjectInitializer& ObjectInitializier = FObjectInitializer::Get());
};