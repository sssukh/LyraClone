#include "LyraCloneEquipmentManagerComponent.h"
#include "LyraCloneEquipmentDefinition.h"
#include "LyraCloneEquipmentInstance.h"
#include "AbilitySystemGlobals.h"
#include "LyraClone/AbilitySystem/LyraCloneAbilitySystemComponent.h"
#include "LyraClone/AbilitySystem/LyraCloneAbilitySet.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneEquipmentManagerComponent)


ULyraCloneAbilitySystemComponent* FLyraCloneEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();

	// GetAbilitySystemComponentFromActor를 잠시 확인해보자:
	// - EquipmentManagerComponent는 ALyraCloneCharacter를 Owner로 가지고 있다.
	// - 해당 함수는 IAbilitySystemInterface를 통해 AbilitySystemComponent를 반환한다.
	// - 우리는 LyraCloneCharacter에 IAbilitySystemInterface를 상속받을 필요가 있다.

	return Cast<ULyraCloneAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}


void FLyraCloneEquipmentList::RemoveEntry(ULyraCloneEquipmentInstance* Instance)
{
	// 단순히 그냥 Entry를 순회하며, Instance를 찾아서
	// iterator를 사용하면 중간 삭제가 이루어져도 iterator 스왑이 필요없음(index는 필요)
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FLyraCloneAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			ULyraCloneAbilitySystemComponent* ASC = GetAbilitySystemComponent();
			check(ASC);
			{
				// TakeFromAbilitySystem은 GivetoAbilitySystem 반대 역할로, ActivatableAbilities에서 제거한다.
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}
			// Actor 제거 작업 및 iterator를 통한 안전하게 Array에서 제거 진행
			Instance->DestroyEquipmentActors();
			EntryIt.RemoveCurrent();
		}
	}
}



ULyraCloneEquipmentManagerComponent::ULyraCloneEquipmentManagerComponent(const FObjectInitializer&
	ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this)
{}

void ULyraCloneEquipmentManagerComponent::UnequipItem(ULyraCloneEquipmentInstance* ItemInstance)
{
	if (ItemInstance)
	{
		// 해당 함수는 BP의 Event노드를 호출해준다
		ItemInstance->OnUnequipped();

		// EquipmentList에 제거해준다
		// - 제거하는 과정을 통해 추가되었던 Actor Instance를 제거를 진행한다.
		EquipmentList.RemoveEntry(ItemInstance);
	}
}

TArray<ULyraCloneEquipmentInstance*> ULyraCloneEquipmentManagerComponent::GetEquipmentInstanceOfType(TSubclassOf<ULyraCloneEquipmentInstance> InstanceType) const
{
	TArray<ULyraCloneEquipmentInstance*> Results;

	// EquipmentList를 순회하며
	for (const FLyraCloneAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (ULyraCloneEquipmentInstance* Instance = Entry.Instance)
		{
			// InstanceType에 맞는 Class이면 Results에 추가하여 반환
			// - 우리의 경우, LyraCloneRangedWeaponInstance가 될거임
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}

ULyraCloneEquipmentInstance* ULyraCloneEquipmentManagerComponent::EquipItem(TSubclassOf<ULyraCloneEquipmentDefinition> EquipmentDefinition)
{
	ULyraCloneEquipmentInstance* Result = nullptr;
	if (EquipmentDefinition)
	{
		Result = EquipmentList.AddEntry(EquipmentDefinition);
		if (Result)
		{
			Result->OnEquipped();
		}
	}
	return Result;
}

ULyraCloneEquipmentInstance* FLyraCloneEquipmentList::AddEntry(TSubclassOf<ULyraCloneEquipmentDefinition>
	EquipmentDefinition)
{
	ULyraCloneEquipmentInstance* Result = nullptr;
	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());

	// EquipmentDefinition의 멤버 변수들은 EditDefaultsOnly로 정의되어 있어 GetDefault로 들고와도 우리에게 필요한 것들이 모두 들어있다.
	// GetDefault를 가져온 이유 :
	// NewObject를 호출(생성)하지 않고 사용할 수 있다.(Default를 가져다 쓰기 떄문)
	const ULyraCloneEquipmentDefinition* EquipmentCDO = GetDefault<ULyraCloneEquipmentDefinition>(EquipmentDefinition);
	TSubclassOf<ULyraCloneEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (!InstanceType)
	{
		InstanceType = ULyraCloneEquipmentInstance::StaticClass();
	}

	// Entries에 추가해주자
	FLyraCloneAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<ULyraCloneEquipmentInstance>(OwnerComponent->GetOwner(),	InstanceType);
	Result = NewEntry.Instance;

	ULyraCloneAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(ASC);
	{
		for (TObjectPtr<ULyraCloneAbilitySet> AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, &NewEntry.GrantedHandles, Result);
		}
	}

	// ActorsToSpawn을 통해, Actor들을 인스턴스화 해주자
	// - 어디에? EquipmentInstance에!
	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);
	return Result;
}
