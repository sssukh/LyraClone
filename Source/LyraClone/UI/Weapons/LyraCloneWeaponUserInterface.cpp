#include "LyraCloneWeaponUserInterface.h"
#include "LyraClone/Equipment/LyraCloneEquipmentManagerComponent.h"
#include "LyraClone/Weapons/LyraCloneWeaponInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneWeaponUserInterface)

ULyraCloneWeaponUserInterface::ULyraCloneWeaponUserInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

void ULyraCloneWeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		if (ULyraCloneEquipmentManagerComponent* EquipmentManager = Pawn -> FindComponentByClass<ULyraCloneEquipmentManagerComponent>())
		{
			if (ULyraCloneWeaponInstance* NewInstance = EquipmentManager -> GetFirstInstanceOfType<ULyraCloneWeaponInstance>())
			{
				if (NewInstance != CurrentInstance && NewInstance->GetInstigator() != nullptr)
				{
					ULyraCloneWeaponInstance* OldWeapon = CurrentInstance;
					CurrentInstance = NewInstance;
					OnWeaponChanged(OldWeapon, CurrentInstance);
				}
			}
		}
	}
}
