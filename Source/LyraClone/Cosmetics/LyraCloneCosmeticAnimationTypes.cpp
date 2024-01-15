#include "LyraCloneCosmeticAnimationTypes.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneCosmeticAnimationTypes)

USkeletalMesh* FLyraCloneAnimBodyStyleSelectionSet::SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const
{
	for (const FLyraCloneAnimBodyStyleSelectionEntry& Rule : MeshRules)
	{
		if ((Rule.Mesh) && CosmeticTags.HasAll(Rule.RequiredTags))
		{
			return Rule.Mesh;
		}
	}
	return DefaultMesh;
}

TSubclassOf<UAnimInstance> FLyraCloneAnimLayerSelectionSet::SelectBestLayer(const FGameplayTagContainer& CosmeticTags) const
{
	for (const FLyraCloneAnimLayerSelectionEntry& Rule : LayerRules)
	{
		if ((Rule.Layer != nullptr) && CosmeticTags.HasAll(Rule.RequiredTags))
			return Rule.Layer;
	}
	return DefaultLayer;
}
