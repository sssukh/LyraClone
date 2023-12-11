#include "LyraClonePlayerState.h"
#include "LyraClone/GameModes/LyraCloneGameMode.h"
#include "LyraClone/GameModes/LyraCloneExperienceManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraClonePlayerState)

ALyraClonePlayerState::ALyraClonePlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}


void ALyraClonePlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	ULyraCloneExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraCloneExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnLyraCloneExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

// 로딩이 완료된 후 Experience를 받아와서 초기화 해주기 위한 함수
void ALyraClonePlayerState::OnExperienceLoaded(const ULyraCloneExperienceDefinition* CurrentExperience)
{
	if (ALyraCloneGameMode* GameMode = GetWorld()->GetAuthGameMode<ALyraCloneGameMode>())
	{
		//ALyraCloneGameMode 에서 GetPawnDataForController를 구현해야 함
		// - ALyraCloneGameMode에서 우리는 아직 PawnData를 설정하지 않았으므로, ExperienceManagerComponent의 DefaultPawnData로 설정한다.
		const ULyraClonePawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}

}

void ALyraClonePlayerState::SetPawnData(const ULyraClonePawnData* InPawnData)
{
	check(InPawnData);

	// PawnData가 두번 설정되는 것은 원하지 않음!
	check(!PawnData);

	PawnData = InPawnData;
}
