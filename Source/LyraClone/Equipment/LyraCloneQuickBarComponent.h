#pragma once
#include "Components/ControllerComponent.h"
#include "LyraCloneQuickBarComponent.generated.h"

class ULyraCloneInventoryItemInstance;
class ULyraCloneEquipmentInstance;
class ULyraCloneEquipmentManagerComponent;

/**
* HUD의 QuickBar를 생각하면 된다:
* - 흔히 MMORPG에서는 ShortCut HUD를 연상하면 된다.
* 
*  해당 Component는 ControllerCOmponent로서, PlayerController에 의해 조작계 중 하나로 생각해도 된다.
* - HUD(Slate)와 Inventory/Equipment(Gameplay)의 다리(Bridge) 역할하는 Component로 생각하자
* - 해당 Component는 Lyra의 HUD 및 Slate Widget을 다루면서 다시 보게될 예정이다.
*/
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class ULyraCloneQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()
public:
	ULyraCloneQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void AddItemToSlot(int32 SlotIndex, ULyraCloneInventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable, Category = "LyraClone")
	void SetActiveSlotIndex(int32 NewIndex);

	ULyraCloneEquipmentManagerComponent* FindEquipmentManager() const;
	void UnequipItemInSlot();
	void EquipItemInSlot();


	// HUD QuickBar Slot 갯수
	UPROPERTY()
	int32 NumSlots = 3;

	// HUD QuickBar Slot 리스트
	UPROPERTY()
	TArray<TObjectPtr<ULyraCloneInventoryItemInstance>> Slots;

	// 현재 활성화된 Slot Index(아마 Lyra는 딱 한개만 Slot이 활성화되는가보다?)
	UPROPERTY()
	int32 ActiveSlotIndex = -1;

	// 현재 장착한 장비 정보
	UPROPERTY()
	TObjectPtr<ULyraCloneEquipmentInstance> EquippedItem;
};
