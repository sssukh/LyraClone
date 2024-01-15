#include "LyraCloneEquipmentDefinition.h"
#include "LyraCloneEquipmentInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneEquipmentDefinition)

ULyraCloneEquipmentDefinition::ULyraCloneEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = ULyraCloneEquipmentInstance::StaticClass();
}
