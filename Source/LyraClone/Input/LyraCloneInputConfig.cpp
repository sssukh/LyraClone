#include "LyraCloneInputConfig.h"
#include "LyraClone/LyraCloneChannels.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneInputConfig)

ULyraCloneInputConfig::ULyraCloneInputConfig(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

const UInputAction* ULyraCloneInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FLyraCloneInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogLyraClone, Error, TEXT("can't find NativeInputAction for InputTag [%s] on InputConfig [% s]."), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}

const UInputAction * ULyraCloneInputConfig::FindAbilityInputActionForTag(const FGameplayTag & InputTag, bool bLogNotFound) const
{
	for (const FLyraCloneInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogLyraClone, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [% s]."), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}