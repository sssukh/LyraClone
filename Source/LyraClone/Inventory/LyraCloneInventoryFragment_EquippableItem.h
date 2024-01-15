#pragma once
#include "LyraCloneInventoryItemDefinition.h"
#include "Templates/SubclassOf.h"
#include "LyraCloneInventoryFragment_EquippableItem.generated.h"

class ULyraCloneEquipmentDefinition;

UCLASS()
class ULyraCloneInventoryFragment_EquippableItem : public ULyraCloneInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = LyraClone)
	TSubclassOf<ULyraCloneEquipmentDefinition> EquipmentDefinition;
};