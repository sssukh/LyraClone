#pragma once

#include "GameFramework/PlayerState.h"
#include "LyraClonePlayerState.generated.h"

class ULyraClonePawnData;
class ULyraCloneExperienceDefinition;

UCLASS()
class ALyraClonePlayerState : public APlayerState
{
	GENERATED_BODY()
	ALyraClonePlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostInitializeComponents() override final;

	UPROPERTY()
	TObjectPtr<const ULyraClonePawnData>	PawnData;
};