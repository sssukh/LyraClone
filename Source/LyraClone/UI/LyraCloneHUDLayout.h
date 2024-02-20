#pragma once

#include "LyraCloneActivatableWidget.h"
#include "LyraCloneHUDLayout.generated.h"

// PrimaryGameLayout의 Layer에 연동할 HUD Layout(CommonActivatableWidget)

UCLASS(Abstract, BlueprintType, Blueprintable,meta =(DisplayName = "LyraClone HUD Layout", Category = "LyraClone|HUD"))
class ULyraCloneHUDLayout : public ULyraCloneActivatableWidget
{
	GENERATED_BODY()
public:
	ULyraCloneHUDLayout(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};