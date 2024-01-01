#pragma once
#include "GameplayTagContainer.h"
#include "LyraCloneCosmeticAnimationTypes.generated.h"

USTRUCT(BlueprintType)
struct FLyraCloneAnimBodyStyleSelectionEntry
{
	GENERATED_BODY()
	
	// AnimLayer를 적용할 대상 SkeletalMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh = nullptr;

	// Cosmetic Tag라고 생각하면 됨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Cosmetic"))
	FGameplayTagContainer RequiredTags;
};USTRUCT(BlueprintType)
struct FLyraCloneAnimBodyStyleSelectionSet
	{
	GENERATED_BODY()
	
	// AnimLayer 적용할 SkeletalMesh를 들고 있음 -> Animation-Mesh 간 Rules를 MeshRules라고 생각하면 됨
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLyraCloneAnimBodyStyleSelectionEntry> MeshRules;
	
	// 그냥 디폴트로 적용할 SkeletalMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> DefaultMesh = nullptr;

	// Physics Assets은 하나로 동일함 -> 즉 모든 Animation의 Physics 속성은 공유함
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPhysicsAsset> ForcedPhysicsAsset = nullptr;
};