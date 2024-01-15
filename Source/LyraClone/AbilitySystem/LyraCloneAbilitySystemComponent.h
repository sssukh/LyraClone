#pragma once

#include "AbilitySystemComponent.h"
#include "LyraCloneAbilitySystemComponent.generated.h"

UCLASS()
class ULyraCloneAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	ULyraCloneAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	* AbilitySystemComponent's interface
	*/
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	/**
	* member methods
	*/
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

	// Ability Input 처리할 Pending Queue
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};