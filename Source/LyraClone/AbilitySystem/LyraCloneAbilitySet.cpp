#include "LyraCloneAbilitySet.h"
#include "LyraClone/AbilitySystem/LyraCloneAbilitySystemComponent.h"
#include "LyraClone/AbilitySystem/Abilities/LyraCloneGameplayAbility.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneAbilitySet)



void FLyraCloneAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FLyraCloneAbilitySet_GrantedHandles::TakeFromAbilitySystem(ULyraCloneAbilitySystemComponent* LyraCloneASC)
{
	if (!LyraCloneASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			//ActivatableAbilities에서 제거한다:
			// - ClearAbility() 함수를 잠깐 보고 오자
			LyraCloneASC->ClearAbility(Handle);
		}
	}
}



ULyraCloneAbilitySet::ULyraCloneAbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void ULyraCloneAbilitySet::GiveToAbilitySystem(ULyraCloneAbilitySystemComponent* LyraCloneASC, FLyraCloneAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject)
{
	check(LyraCloneASC);

	if (!LyraCloneASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	// gameplay abilities를 허용:
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FLyraCloneAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];
		if (!IsValid(AbilityToGrant.Ability))
		{
			continue;
		}

		// GiveAbility()에서 만약 EGameplayAbilityInstancingPolicy::InstancedPerActor라면, 내부적으로 Instance를 생성하지만 그렇지 않으면 CDO를 할당한다.
		// - 이를 통해 UObject 갯수를 줄여 UObject가 늘어남에 따라 늘어나는 성능적/메모리적 부하를 줄일 수 있다.
		ULyraCloneGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<ULyraCloneGameplayAbility>();

		// AbilitySpec은 giveAbility로 전달되며, ActivatableAbilities에 추가된다.
		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		// GiveAbility()를 잠깐 살펴보도록 하자:
		const FGameplayAbilitySpecHandle AbilitySpecHandle = LyraCloneASC->GiveAbility(AbilitySpec);
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}
}
