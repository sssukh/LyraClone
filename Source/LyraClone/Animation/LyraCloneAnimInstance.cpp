#include "LyraCloneAnimInstance.h"
#include "AbilitySystemGlobals.h"
#include "LyraClone/AbilitySystem/LyraCloneAbilitySystemComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneAnimInstance)

ULyraCloneAnimInstance::ULyraCloneAnimInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

void ULyraCloneAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	// ASC 내부 관리하는 GameplayTag와 AnimInstance의 멤버 Property와 Delegate를 연결하여, 값 변화에 대한 반영을 진행한다
	GameplayTagPropertyMap.Initialize(this, ASC);
}


void ULyraCloneAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(ASC);
		}
	}
}

