#include "LyraCloneExperienceManagerComponent.h"
#include "LyraCloneExperienceDefinition.h"
#include "LyraCloneExperienceActionSet.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeaturesSubsystemSettings.h"
#include "Net/UnrealNetwork.h"
#include "LyraClone/System/LyraCloneAssetManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneExperienceManagerComponent)

ULyraCloneExperienceManagerComponent::ULyraCloneExperienceManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

/* 
* 사용할 Experience가 로딩이 완료되었다면 Experience를 델리게이트를 통해 전달하고 아직 로딩이 완료되어 있지 않다면
* 데릴게이트에 등록하는 함수
* @param Delegate 등록할 델리게이트
*/
void ULyraCloneExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnLyraCloneExperienceLoaded::FDelegate&& Delegate)
{
	// Experience의 로딩이 완료되었다면 델리게이트를 통해 전달
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	// Experience의 로딩이 완료되지 않았다면 델리게이트에 등록
	else
	{
		// Delegate의 경우 내부적으로 필요한 메모리를 할당해놓기 때문에 복사 비용을 낮추기 위해 Move를 통해 옮겨야 한다.
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}

void ULyraCloneExperienceManagerComponent::ServerSetCurrentExperience(FPrimaryAssetId ExperienceId)
{
	ULyraCloneAssetManager& AssetManager = ULyraCloneAssetManager::Get();

	TSubclassOf<ULyraCloneExperienceDefinition> AssetClass;
	{
		FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
		// 클래스 로딩 이유는 블루프린트이기 때문에
		AssetClass = Cast<UClass>(AssetPath.TryLoad());
	}

	//왜 CDO를 가져오는 걸까?
	const ULyraCloneExperienceDefinition* Experience = GetDefault<ULyraCloneExperienceDefinition>(AssetClass);
	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	{
		// 그리고 CDO로 CurrentExperience를 설정한다!
		// 어떤 의도로 어떻게 코드를 작성한지는 코드를 쭉 읽어보고(StartExperienceLoad까지 읽어보자) 다시 생각해보자
		CurrentExperience = Experience;
	}

	StartExperienceLoad();
}

void ULyraCloneExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience);
	check(LoadState == ELyraCloneExperienceLoadState::Unloaded);

	LoadState = ELyraCloneExperienceLoadState::Loading;

	ULyraCloneAssetManager& AssetManager = ULyraCloneAssetManager::Get();

	TSet<FPrimaryAssetId> BundleAssetList;

	// 이미 앞서, ServerSetCurrentExperience에서 우리는 ExperienceId를 넘겨주었는데, 여기서 CDO를 활용하여, GetPrimaryAssetId를 로딩할 대상으로 넣는다.
	// 왜 이렇게 하는걸까?
	// GetPrimaryAssetId를 살펴봄으로써, 아래의 두가리를 알 수 있다.
	// 1. 우리는 B_LyraCloneDefaultExperience를 BP로 만든 이유
	// 2. CDO를 가져와서, GetPrimaryAssetId를 호출한 이유

	// 우리는 앞서 이미 CDO로 로딩하여, CDO를 사용하지 않고 CDO를 사용하여 로딩할 에셋을 지정하여, BundleAssetList에 추가해준다.
	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());

	// load assets associated with the experience
	// 아래는 우리가 후일 GameFeature를 사용하여, Experience에 바인딩된 GameFeature Plugin을 로딩할 Bundle 이름을 추가한다.
	// - Bundle이라는게 후일 우리가 로딩할 에셋의 카테고리 이름이고 생각하면 된다 (일단 지금은 넘어가자 후일 ,또 다룰 것이다)
	TArray<FName> BundlesToLoad;
	{
		// 여기서 주목해야 할 부분은 OwnerNetMode가 NM_Standalone이면? Client/Server 둘다 로딩에 추가된다.
		const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
		bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
		bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);
		if (bLoadClient)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
		}
		if (bLoadServer)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
		}
	}

	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);

	// 아래도, 후일 Bundle을 우리가 GameFeature에 연동하면서 더 깊게 알아보기로 하고, 지금은 앞서 B_LyraCloneDefaultExperience를 로딩해주는 ㅎ마수로 생각하자
	TSharedPtr<FStreamableHandle> Handle = AssetManager.ChangeBundleStateForPrimaryAssets(
		BundleAssetList.Array(),
		BundlesToLoad,
		{}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);

	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		// 로딩이 완료되었으면, ExecuteDelegate를 통해 OnAssetsLoadedDelegate를 호출하자:
		// - 아래의 함수를 확인해보자:
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);
		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
			{
				OnAssetsLoadedDelegate.ExecuteIfBound();
			}));
	}

	// FrameNumber를 주목해서 보자
	static int32 StartExperienceLoad_FrameNumber = GFrameNumber;
}

void ULyraCloneExperienceManagerComponent::OnExperienceLoadComplete()
{
	// FrameNumber를 주목해서 보자
	static int32 OnExperienceLoadComplete_FrameNumber = GFrameNumber;

	check(LoadState == ELyraCloneExperienceLoadState::Loading);
	check(CurrentExperience);

	GameFeaturePluginURLs.Reset();
	auto CollectGameFeaturePluginURLs = [This = this](const UPrimaryDataAsset* Context, const TArray<FString>& FeaturePluginList)
	{
		for (const FString& PluginName : FeaturePluginList)
		{
			FString PluginURL;
			if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, PluginURL))
			{
				This->GameFeaturePluginURLs.AddUnique(PluginURL);
			}
		}
	};
	CollectGameFeaturePluginURLs(CurrentExperience, CurrentExperience->GameFeaturesToEnable);
	NumGameFeaturePluginsLoading = GameFeaturePluginURLs.Num();
	if (NumGameFeaturePluginsLoading)
	{
		LoadState = ELyraCloneExperienceLoadState::LoadingGameFeatures;
		for (const FString& PluginURL : GameFeaturePluginURLs)
		{
			UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(PluginURL,
				FGameFeaturePluginLoadComplete::CreateUObject(this, &ThisClass::OnGameFeaturePluginLoadComplete));
		}
	}
	else
	{
		OnExperienceFullLoadCompleted();
	}
}

void ULyraCloneExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != ELyraCloneExperienceLoadState::Loaded);
	{
		LoadState = ELyraCloneExperienceLoadState::ExecutingActions;
		FGameFeatureActivatingContext Context;
		{
			const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
			if (ExistingWorldContext)
			{
				Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
			}
		}
		auto ActivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
		{
			for (UGameFeatureAction* Action : ActionList)
			{
				if (Action)
				{
					Action->OnGameFeatureRegistering();
					Action->OnGameFeatureLoading();
					Action->OnGameFeatureActivating(Context);
				}
			}
		};
		ActivateListOfActions(CurrentExperience->Actions);
		for (const TObjectPtr<ULyraCloneExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
		{
			ActivateListOfActions(ActionSet->Actions);
		}
	}

	LoadState = ELyraCloneExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}

const ULyraCloneExperienceDefinition* ULyraCloneExperienceManagerComponent::GetCurrentExperienceChecked() const
{
	check(LoadState == ELyraCloneExperienceLoadState::Loaded);
	check(CurrentExperience != nullptr);
	return CurrentExperience;
}

void ULyraCloneExperienceManagerComponent::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
	NumGameFeaturePluginsLoading--;
	if (NumGameFeaturePluginsLoading == 0)
	{
		OnExperienceFullLoadCompleted();
	}

}
