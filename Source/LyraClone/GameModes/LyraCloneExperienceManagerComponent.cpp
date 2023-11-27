#include "LyraCloneExperienceManagerComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneExperienceManagerComponent)

ULyraCloneExperienceManagerComponent::ULyraCloneExperienceManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void ULyraCloneExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnLyraCloneExperienceLoaded::FDelegate&& Delegate)
{
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}