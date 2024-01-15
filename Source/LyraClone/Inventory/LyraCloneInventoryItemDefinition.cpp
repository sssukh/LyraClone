#include "LyraCloneInventoryItemDefinition.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneInventoryItemDefinition)

ULyraCloneInventoryItemDefinition::ULyraCloneInventoryItemDefinition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

const ULyraCloneInventoryItemFragment* ULyraCloneInventoryItemDefinition::FindFragmentByClass(TSubclassOf<ULyraCloneInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass)
	{
		for (ULyraCloneInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}
	return nullptr;
}