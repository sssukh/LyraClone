#include "LyraCloneAssetManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneAssetManager)

ULyraCloneAssetManager::ULyraCloneAssetManager() : Super()
{

}

ULyraCloneAssetManager& ULyraCloneAssetManager::Get()
{
	check(GEngine);

	if (ULyraCloneAssetManager* Singleton = Cast<ULyraCloneAssetManager>(GEngine->AssetManager))
		return *Singleton;

	UE_LOG(LogTemp, Fatal, TEXT("invalid AssetManagerClassname in DefaultEngine.ini(project setting);"" it must be LyraCloneAssetManager"));

	return *NewObject<ULyraCloneAssetManager>();
}


void ULyraCloneAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UE_LOG(LogTemp, Warning, TEXT("StartInitialLoading"));
}

bool ULyraCloneAssetManager::ShouldLogAssetLoads()
{
	const TCHAR* commnadLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param(commnadLineContent, TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}


void ULyraCloneAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}

UObject* ULyraCloneAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;

		if (ShouldLogAssetLoads())
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("synchronous loaded assets [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);

		if (UAssetManager::IsValid())
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);

		return AssetPath.TryLoad();
	}

	return nullptr;
}
void ULyraCloneAssetManager::TestClone()
{
	ShouldLogAssetLoads();
}

