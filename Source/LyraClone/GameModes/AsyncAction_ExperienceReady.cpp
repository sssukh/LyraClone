#include "AsyncAction_ExperienceReady.h"
#include "LyraCloneExperienceManagerComponent.h"
#include "LyraCloneExperienceDefinition.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(AsyncAction_ExperienceReady)

UAsyncAction_ExperienceReady::UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


UAsyncAction_ExperienceReady* UAsyncAction_ExperienceReady::WaitForExperienceReady(UObject* WorldContextObject)
{
	UAsyncAction_ExperienceReady* Action = nullptr;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,
		EGetWorldErrorMode::LogAndReturnNull))
	{
		Action = NewObject<UAsyncAction_ExperienceReady>();
		Action->WorldPtr = World;
		Action->RegisterWithGameInstance(World);
	}
	return Action;
}void UAsyncAction_ExperienceReady::Activate()
{
	if (UWorld* World = WorldPtr.Get())
	{
		// GameState가 이미 World에 준비되어 있으면, Step1을 스킵하고 바로 Step2를 진행한다.
		if (AGameStateBase* GameState = World->GetGameState())
		{
			Step2_ListenToExperienceLoading(GameState);
		}
		else
		{
			// 아직 준비되어 있지 않으면, UWorld::GameStateSetEvent에 Step1_HandleGameStateSet을 Delegate로 바인딩시킨다.
			// - Step1부터 시작할 것이다
			World->GameStateSetEvent.AddUObject(this, &ThisClass::Step1_HandleGameStateSet);
		}
	}
	else
	{
		// 현재 AsyncAction 대상이었던 World가 더이상 Valid하지 않으므로, 제거 진행
		// - 코드를 내려가보면, GameInstance에서 레퍼런스 대상으로 제거시킨다
		// - UAsyncAction의 캐싱 위치가 GameInstance임을 알 수 있다.
		SetReadyToDestroy();
	}
}

void UAsyncAction_ExperienceReady::Step1_HandleGameStateSet(AGameStateBase* GameState)
{
	if (UWorld* World = WorldPtr.Get())
	{
		World->GameStateSetEvent.RemoveAll(this);
	}
	Step2_ListenToExperienceLoading(GameState);
}void UAsyncAction_ExperienceReady::Step2_ListenToExperienceLoading(AGameStateBase* GameState)
{
	check(GameState);
	// 여기서 왜 우리가 GameStateSetEvent에 Delegate로 바인딩시켰는지 이유가 나온다.
	// - Experience 로딩 상태 정보가 GameState에 붙어있는 ExperienceManagerComponent에서 가져올 수 있기 때문이다.
	ULyraCloneExperienceManagerComponent* ExperienceManagerComponent = GameState -> FindComponentByClass<ULyraCloneExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// 만약 이미 Experience가 준비되었다면, Step3을 스킵하고 step4로 간다.
	if (ExperienceManagerComponent->IsExperienceLoaded())
	{
		UWorld* World = GameState->GetWorld();
		check(World);
		// 아마 Experience가 준비되었다고 해도, 아직 준비되지 않은 상태가 있을 수도 있으니 (혹은 과정중이라던가?) 그래서 그냥 다음 Tick에 실행되도록 Timer를 바인딩시킨다..
		World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &ThisClass::Step4_BroadcastReady));
	}
	else
	{
		// 준비가 안되었다면, OnExperienceLoaded에 바인딩시킨다(후일 로딩 끝나면 Step3부터 실행이 될 것이다).
		ExperienceManagerComponent -> CallOrRegister_OnExperienceLoaded(FOnLyraCloneExperienceLoaded::FDelegate::CreateUObject(this,&ThisClass::Step3_HandleExperienceLoaded));
	}
}

void UAsyncAction_ExperienceReady::Step3_HandleExperienceLoaded(const ULyraCloneExperienceDefinition*
	CurrentExperience)
{
	// 현재 따로 ExperienceDefinition에 대해 처리할 로직은 없다
	Step4_BroadcastReady();
}void UAsyncAction_ExperienceReady::Step4_BroadcastReady()
{
	// 바인딩된 BP 혹은 UFUNCTION을 호출해준다.
	OnReady.Broadcast();
	SetReadyToDestroy();
}