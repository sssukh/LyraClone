#include "LyraCloneGameplayAbility.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneGameplayAbility)

ULyraCloneGameplayAbility::ULyraCloneGameplayAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ActivationPolicy = ELyraCloneAbilityActivationPolicy::OnInputTriggered;
}
