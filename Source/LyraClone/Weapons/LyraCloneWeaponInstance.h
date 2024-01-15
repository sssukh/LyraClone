#pragma once

#include "LyraClone/Equipment/LyraCloneEquipmentInstance.h"
#include "LyraClone/Cosmetics/LyraCloneCosmeticAnimationTypes.h"
#include "LyraCloneWeaponInstance.generated.h"

UCLASS()
class ULyraCloneWeaponInstance : public ULyraCloneEquipmentInstance
{
	GENERATED_BODY()
public:
	ULyraCloneWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Weapon에 적용할 AnimLayer를 선택하여 반환
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Animation)
	TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FLyraCloneAnimLayerSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FLyraCloneAnimLayerSelectionSet UnequippedAnimSet;
};
