#include "LyraCloneWeaponInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneWeaponInstance)

ULyraCloneWeaponInstance::ULyraCloneWeaponInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

TSubclassOf<UAnimInstance> ULyraCloneWeaponInstance::PickBestAnimLayer(bool bEquipped, const
	FGameplayTagContainer& CosmeticTags) const
{
	const FLyraCloneAnimLayerSelectionSet& SetToQuery = (bEquipped ? EquippedAnimSet : UnequippedAnimSet);
	return SetToQuery.SelectBestLayer(CosmeticTags);
}