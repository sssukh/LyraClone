#pragma once
#include "UObject/Object.h"
#include "UObject/UObjectGlobals.h"
#include "Containers/Array.h"
#include "LyraCloneEquipmentDefinition.h"
#include "LyraCloneEquipmentInstance.generated.h"

UCLASS(BlueprintType, Blueprintable)
class ULyraCloneEquipmentInstance : public UObject
{
	GENERATED_BODY()
public:
	ULyraCloneEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	* Blueprint 정의를 위한 Equip/Unequip 함수
	*/

	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnEquipped"))
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnUnequipped"))
	void K2_OnUnequipped();

	// interface
	virtual void OnEquipped();
	virtual void OnUnequipped();

	void SpawnEquipmentActors(const TArray<FLyraCloneEquipmentActorToSpawn>& ActorsToSpawn);
	void DestroyEquipmentActors();

	

	UFUNCTION(BlueprintPure, Category = Equipment)
	APawn* GetPawn() const;

	// DeterminesOutputType은 C++ 정의에는 APawn* 반환하지만, BP에서는 PawnType에 따라 OutputType이 결정되도록 리다이렉트(Redirect)한다
	UFUNCTION(BlueprintPure, Category = Equipment, meta = (DeterminesOutputType = PawnType))
	APawn* GetTypedPawn(TSubclassOf<APawn> PawnType) const;

	UFUNCTION(BlueprintPure,Category = Equipment)
	TArray<AActor*> GetSpawnedActors() const {return SpawnedActors;}


	// 어떤 InventoryItemInstance에 의해 활성화되었는지 (추후, QuickBarComponent에서 보게 될 것이다)
	UPROPERTY()
	TObjectPtr<UObject> Instigator;

	// LyraCloneEquipmentDefinition에 맞게 Spawn된 Actor Instance들
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;

};
