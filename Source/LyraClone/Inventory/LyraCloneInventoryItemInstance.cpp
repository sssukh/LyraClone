#include "LyraCloneInventoryItemInstance.h"
#include "LyraCloneInventoryItemDefinition.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneInventoryItemInstance)

ULyraCloneInventoryItemInstance::ULyraCloneInventoryItemInstance(const FObjectInitializer& ObjectInitializer) :	Super(ObjectInitializer)
{}

const ULyraCloneInventoryItemFragment* ULyraCloneInventoryItemInstance::FindFragmentByClass(TSubclassOf<ULyraCloneInventoryItemFragment> FragmentClass) const
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		// LyraCloneInventoryItemDefinition은 모든 멤버 변수가 EditDefaultsOnly로 선언되어 있으므로, GetDefault로 가져와도 무관하다.
		// - Fragment 정보는 Instance가 아닌 Definition에 있다.
		return GetDefault<ULyraCloneInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}