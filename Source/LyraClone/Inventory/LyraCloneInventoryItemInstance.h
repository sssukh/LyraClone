#pragma once
#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "LyraCloneInventoryItemInstance.generated.h"

class ULyraCloneInventoryItemDefinition;
class ULyraCloneInventoryItemFragment;

// 해당 클래스는 Inventory Item의 인스턴스로 볼 수 있다.
UCLASS(BlueprintType)
class ULyraCloneInventoryItemInstance : public UObject
{
	GENERATED_BODY()
	public:
	ULyraCloneInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const ULyraCloneInventoryItemFragment*	FindFragmentByClass(TSubclassOf<ULyraCloneInventoryItemFragment> FragmentClass) const;

	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}


	// Inventory Item의 인스턴스에는 무엇으로 정의되었는지 메타 클래스인 LyraCloneInventoryItemDefinition을 들고 있다.
	UPROPERTY()
	TSubclassOf<ULyraCloneInventoryItemDefinition> ItemDef;
};