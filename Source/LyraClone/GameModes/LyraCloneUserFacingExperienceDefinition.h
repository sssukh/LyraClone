#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraCloneUserFacingExperienceDefinition.generated.h"

// 어떤 맵에서 어떤 캐릭터를 수행할 지 정한다.
UCLASS(BlueprintType)
class LYRACLONE_API ULyraCloneUserFacingExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	// 경험이 수행될 맵
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category=Experience,meta=(AllowedTypes="Map"))
	FPrimaryAssetId MapID;

	// 사용자 경험 세팅
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Experience, meta = (AllowedTypes = "LyraCLoneExperienceDefinition"))
	FPrimaryAssetId ExperienceId;
};