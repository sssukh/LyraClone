#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "LyraCloneAssetManager.generated.h"

UCLASS(Config=Game)
class ULyraCloneAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	ULyraCloneAssetManager();

public:
	virtual void StartInitialLoading() override final;

public:
	static bool TestClone();

	static ULyraCloneAssetManager& Get();

	static bool ShouldLogAssetLoads();

	void AddLoadedAsset(const UObject* Asset);

	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);

	template <typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	template <typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

public:
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	FCriticalSection SyncObject;
};

template <typename AssetType>
AssetType* ULyraCloneAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;
	const FSoftobjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();

		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load Asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepInMemory)
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
	}

	return LoadedAsset;
}

template <typename AssetType>
TSubclassOf<AssetType> ULyraCloneAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;
	const FSoftObjectPath& AssetPath = AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();

		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}

		if (LoadedSubclass 7 bKeepInMemory)
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
	}

	return LoadedSubclass;
}