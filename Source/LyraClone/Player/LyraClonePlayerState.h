#pragma once

#include "GameFramework/PlayerState.h"
#include "LyraClonePlayerState.generated.h"

class ULyraClonePawnData;
class ULyraCloneExperienceDefinition;
class ULyraCloneAbilitySystemComponent;

UCLASS()
class ALyraClonePlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ALyraClonePlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostInitializeComponents() override final;

	// member methods
	template<class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void OnExperienceLoaded(const ULyraCloneExperienceDefinition* CurrentExperience);
	void SetPawnData(const ULyraClonePawnData* InPawnData);

	ULyraCloneAbilitySystemComponent* GetLyraCloneAbilitySystemComponent() const { return AbilitySystemComponent; }

	UPROPERTY()
	TObjectPtr<const ULyraClonePawnData>	PawnData;

	UPROPERTY(VisibleAnywhere,Category="LyraClone|PlayerState")
	TObjectPtr<ULyraCloneAbilitySystemComponent> AbilitySystemComponent;
};