#pragma once
#include "Components/PawnComponent.h"
#include "LyraClone/AbilitySystem/LyraCloneAbilitySet.h"
#include "LyraCloneEquipmentManagerComponent.generated.h"

class ULyraCloneEquipmentDefinition;
class ULyraCloneEquipmentInstance;

USTRUCT(BlueprintType)
struct FLyraCloneAppliedEquipmentEntry
{
	GENERATED_BODY()
	// 장착물에 대한 메타 데이터
	UPROPERTY()
	TSubclassOf<ULyraCloneEquipmentDefinition> EquipmentDefinition;

	// EquipmentDefinition을 통해 생성된 인스턴스
	UPROPERTY()
	TObjectPtr<ULyraCloneEquipmentInstance> Instance = nullptr;

	// 무기에 할당된 허용가능한 GameplayAbility
	UPROPERTY()
	FLyraCloneAbilitySet_GrantedHandles GrantedHandles;
};

/***
* 참고로 EquipmentInstance의 인스턴스를 Entry에서 관리하고 있다.
* - LyraCloneEquipmnetList는 생성된 객체를 관리한다고 보면 된다.
*/
USTRUCT(BlueprintType)
struct FLyraCloneEquipmentList
{
	GENERATED_BODY()
	FLyraCloneEquipmentList(UActorComponent* InOwnerComponent = nullptr)
		: OwnerComponent(InOwnerComponent)
	{}

	ULyraCloneEquipmentInstance* AddEntry(TSubclassOf<ULyraCloneEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(ULyraCloneEquipmentInstance* Instance);

	ULyraCloneAbilitySystemComponent* GetAbilitySystemComponent() const;

	// 장착물에 대한 관리 리스트
	UPROPERTY()
	TArray<FLyraCloneAppliedEquipmentEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

// Pawn의 Component로서 장착물에 대한 관리를 담당한다.
UCLASS(BlueprintType)
class ULyraCloneEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
public:
	ULyraCloneEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ULyraCloneEquipmentInstance* EquipItem(TSubclassOf<ULyraCloneEquipmentDefinition> EquipmentDefinition);
	void UnequipItem(ULyraCloneEquipmentInstance* ItemInstance);

	UFUNCTION(BlueprintCallable)
	TArray<ULyraCloneEquipmentInstance*> GetEquipmentInstanceOfType(TSubclassOf<ULyraCloneEquipmentInstance> InstanceType) const;
	

	UPROPERTY()
	FLyraCloneEquipmentList EquipmentList;
};
