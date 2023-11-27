// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LyraCloneGameMode.generated.h"

class ULyraCloneExperienceDefinition;
/**
 * 
 */
UCLASS()
class LYRACLONE_API ALyraCloneGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ALyraCloneGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override final;
	virtual void InitGameState() override final;
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	void HandleMatchAssignmentIfNotExpectingOne();
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId);
	bool IsExperienceLoaded() const;
	void OnExperienceLoaded(const ULyraCloneExperienceDefinition* CurrentExperience);
	

	

};
