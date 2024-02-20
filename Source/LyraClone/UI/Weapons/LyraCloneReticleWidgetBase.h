#pragma once

#include "CommonUserWidget.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "LyraCloneReticleWidgetBase.generated.h"

class ULyraCloneWeaponInstance;
class ULyraCloneInventoryItemInstance;

UCLASS(Abstract)
class ULyraCloneReticleWidgetBase : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	ULyraCloneReticleWidgetBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void InitializeFromWeapon(ULyraCloneWeaponInstance* InWeapon);
	
	// WeaponInstance/InventoryInstance를 상태 추적용으로 캐싱 목적
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULyraCloneWeaponInstance> WeaponInstance;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULyraCloneInventoryItemInstance> InventoryInstance;
};