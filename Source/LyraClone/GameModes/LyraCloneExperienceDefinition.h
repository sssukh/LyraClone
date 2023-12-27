#pragma once

#include "Engine/DataAsset.h"
#include "LyraCloneExperienceDefinition.generated.h"

class ULyraClonePawnData;
class UGameFeatureAction;
class ULyraCloneExperienceActionSet;

UCLASS()
class ULyraCloneExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	ULyraCloneExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TObjectPtr<ULyraClonePawnData> DefaultPawnData;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<TObjectPtr<ULyraCloneExperienceActionSet>> ActionSets;

	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};