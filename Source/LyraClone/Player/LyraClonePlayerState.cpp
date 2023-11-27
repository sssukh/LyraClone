#include "LyraClonePlayerState.h"
//#include "LyraClone/GameModes/LyraCloneExperienceManagerComponents.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraClonePlayerState)

ALyraClonePlayerState::ALyraClonePlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}


//void ALyraClonePlayerState::PostInitializeComponents()
//{
//	Super::PostInitializeComponents();
//
//	const AGameStateBase* GameState = GetWorld()->GetGameState();
//	check(GameState);
//
//	ULyraCloneExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraCloneExperienceManagerComponent>();
//	check(ExperienceManagerComponent);
//
//	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnLyraCloneExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
//}
//
//void ALyraClonePlayerState::OnExperienceLoaded(const ULyraCloneExperienceDefinition* CurrentExperience)
//{
//
//}