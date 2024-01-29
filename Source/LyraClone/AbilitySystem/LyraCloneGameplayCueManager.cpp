#include "LyraCloneGameplayCueManager.h"
#include "AbilitySystemGlobals.h"
#include "GameplayCueSet.h"
#include "LyraClone/System/LyraCloneAssetManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneGameplayCueManager)

ULyraCloneGameplayCueManager* ULyraCloneGameplayCueManager::ULyraCloneGameplayCueManager::Get()
{
	return Cast<ULyraCloneGameplayCueManager>(UAbilitySystemGlobals::Get().GetGameplayCueManager());
	
}

ULyraCloneGameplayCueManager::ULyraCloneGameplayCueManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

const FPrimaryAssetType UFortAssetManager_GameplayCueRefsType = TEXT("GameplayCueRefs");
const FName UFortAssetManager_GameplayCueRefsName = TEXT("GameplayCueReferences");
const FName UFortAssetManager_LoadStateClient = FName(TEXT("Client"));

void ULyraCloneGameplayCueManager::RefreshGameplayCuePrimaryAsset()
{
	TArray<FSoftObjectPath> CuePaths;
	UGameplayCueSet* RuntimeGameplayCueSet = GetRuntimeCueSet();
	if (RuntimeGameplayCueSet)
	{
		RuntimeGameplayCueSet->GetSoftObjectPaths(CuePaths);
	}

	// 새로 추가도니 에셋의 경로를 CuePath에 추가하여!
	FAssetBundleData BundleData;
	BundleData.AddBundleAssetsTruncated(UFortAssetManager_LoadStateClient, CuePaths);

	// PrimaryAssetId를 고정된 이름으로 설정하여 (const로 설정되어 있음)
	FPrimaryAssetId PrimaryAssetId = FPrimaryAssetId(UFortAssetManager_GameplayCueRefsType, UFortAssetManager_GameplayCueRefsName);

	// 애셋 매니저에 추가 진행
	UAssetManager::Get().AddDynamicAsset(PrimaryAssetId, FSoftObjectPath(), BundleData);
}
