#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraCloneExperienceActionSet.generated.h"

class UGameFeatureAction;

UCLASS(BlueprintType)
class ULyraCloneExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	ULyraCloneExperienceActionSet();

	UPROPERTY(EditAnywhere, Category = "Actions to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};