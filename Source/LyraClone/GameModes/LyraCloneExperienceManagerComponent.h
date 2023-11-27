#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "LyraCloneExperienceManagerComponent.generated.h"

class ULyraCloneExperienceDefinition;

enum class ELyraCloneExperienceLoadState
{
	Unloaded,
	Loading,
	Loaded,
	Deactivating
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLyraCloneExperienceLoaded, const ULyraCloneExperienceDefinition*);

UCLASS()
class ULyraCloneExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
	public:
	ULyraCloneExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	bool IsExperienceLoaded() const { return (LoadState == ELyraCloneExperienceLoadState::Loaded) && (CurrentExperience != nullptr); }

	void CallOrRegister_OnExperienceLoaded(FOnLyraCloneExperienceLoaded::FDelegate&& Delegate);

	UPROPERTY()
	TObjectPtr<const ULyraCloneExperienceDefinition> CurrentExperience;

	ELyraCloneExperienceLoadState LoadState = ELyraCloneExperienceLoadState::Unloaded;

	FOnLyraCloneExperienceLoaded OnExperienceLoaded;
};