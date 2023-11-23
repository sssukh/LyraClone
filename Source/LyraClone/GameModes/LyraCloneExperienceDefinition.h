#pragma once

#include "Engine/DataAsset.h"
#include "LyraCloneExperienceDefinition.generated.h"

class ULyraClonePawnData;

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
};