#include "LyraCloneAssetManager.h"
#include "LyraClone/LyraCloneGameplayTags.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneAssetManager)


bool ULyraCloneAssetManager::TestClone()
{
	static bool bResult = false;
	{
		bResult = ShouldLogAssetLoads();
	}
	
	return true;
}

ULyraCloneAssetManager& ULyraCloneAssetManager::Get()
{
	check(GEngine);

	if (ULyraCloneAssetManager* Singleton = Cast<ULyraCloneAssetManager>(GEngine->AssetManager))
		return *Singleton;

	UE_LOG(LogTemp, Fatal, TEXT("invalid AssetManagerClassname in DefaultEngine.ini(project setting);"" it must be LyraCloneAssetManager"));

	return *NewObject<ULyraCloneAssetManager>();
}

bool ULyraCloneAssetManager::ShouldLogAssetLoads()
{
	const TCHAR* commnadLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param(commnadLineContent, TEXT("LogAssetLoads"));
	return bLogAssetLoads;
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

ULyraCloneAssetManager::ULyraCloneAssetManager() : UAssetManager()
{

}

void ULyraCloneAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}


void ULyraCloneAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FLyraCloneGameplayTags::InitializeNativeTags();
}






