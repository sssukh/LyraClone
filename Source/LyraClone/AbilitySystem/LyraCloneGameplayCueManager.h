#pragma once

#include "GameplayCueManager.h"
#include "LyraCloneGameplayCueManager.generated.h"

UCLASS()
class ULyraCloneGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()
public:
	static ULyraCloneGameplayCueManager* Get();

	ULyraCloneGameplayCueManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void RefreshGameplayCuePrimaryAsset();
};