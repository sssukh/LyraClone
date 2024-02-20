#pragma once

#include "CommonUserWidget.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "LyraCloneWeaponUserInterface.generated.h"

class ULyraCloneWeaponInstance;

UCLASS()
class ULyraCloneWeaponUserInterface : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	ULyraCloneWeaponUserInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Weapon 변경에 따른 BP Event
	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponChanged(ULyraCloneWeaponInstance* OldWeapon, ULyraCloneWeaponInstance* NewWeapon);

	// UUSerWidget's interface
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 현재 장식된 WeaponInstance를 추적한다(NativeTick을 활용하여 주기적으로 업데이트한다) -> 그렇게 좋은 방법은 아닌것 같다?
	UPROPERTY(Transient)
	TObjectPtr<ULyraCloneWeaponInstance> CurrentInstance;
};