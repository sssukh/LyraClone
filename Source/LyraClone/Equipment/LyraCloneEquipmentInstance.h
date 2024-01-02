#pragma once
#include "UObject/Object.h"
#include "LyraCloneEquipmentInstance.generated.h"

UCLASS(BlueprintType, Blueprintable)
class ULyraCloneEquipmentInstance : public UObject
{
	GENERATED_BODY()
public:
	ULyraCloneEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};