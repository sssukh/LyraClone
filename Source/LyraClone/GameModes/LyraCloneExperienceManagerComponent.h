#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "LyraCloneExperienceManagerComponent.generated.h"

class ULyraCloneExperienceDefinition;

enum class ELyraCloneExperienceLoadState
{
	Unloaded,
	Loading,
	Loaded,
	Deactivating
};

// 로딩이 완료된 Experience를 전달하는 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLyraCloneExperienceLoaded, const ULyraCloneExperienceDefinition*);

UCLASS()
class ULyraCloneExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
	public:
	ULyraCloneExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 현재 Experience가 로딩이 다 되었는지 체크하는 함수
	bool IsExperienceLoaded() const { return (LoadState == ELyraCloneExperienceLoadState::Loaded) && (CurrentExperience != nullptr); }

	// OnExperienceLoaded에 바인딩하거나, 이미 Experience 로딩이 완료되었다면 바로 호출함
	void CallOrRegister_OnExperienceLoaded(FOnLyraCloneExperienceLoaded::FDelegate&& Delegate);
	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnExperienceFullLoadComplete();

	/*
	* member variable
	*/

	// 참고로 해당 멤버변수는 lyra에서는 'ReplcatedUsing=' 으로 선언되어있따 :
	// - 현재 우리는 아직 Replication을 신경쓰지 않을 것이기에, 최대한 네트워크 서버 코드를 배제하도록 하겠다.
	UPROPERTY()
	TObjectPtr<const ULyraCloneExperienceDefinition> CurrentExperience;

	ELyraCloneExperienceLoadState LoadState = ELyraCloneExperienceLoadState::Unloaded;

	FOnLyraCloneExperienceLoaded OnExperienceLoaded;
};