#pragma once

#include "GameFramework/GameStateBase.h"
#include "LyraCLoneExperienceManagerComponent.h"
#include "LyraCloneGameState.generated.h"

UCLASS()
class ALyraCloneGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ALyraCloneGameState(const FObjectInitializer& ObjectInitializier = FObjectInitializer::Get());

	UPROPERTY()
	TObjectPtr<ULyraCloneExperienceManagerComponent> ExperienceManagerComponent;
};