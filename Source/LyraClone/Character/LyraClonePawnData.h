#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraClone/Camera/LyraCloneCameraMode.h"
#include "LyraClone/Input/LyraCloneInputConfig.h"
#include "LyraClonePawnData.generated.h"

class ULyraCloneAbilitySet;

// 사용할 폰이 가질 최소한의 것을 정의하는 곳
UCLASS(BlueprintType)
class ULyraClonePawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	ULyraClonePawnData(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LyraClone|Pawn")
	TSubclassOf<APawn> PawnClass;

	// Camera Mode
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "LyraClone|Camera")
	TSubclassOf<ULyraCloneCameraMode> DefaultCameraMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LyraClone|InputConfig")
	TObjectPtr<ULyraCloneInputConfig> InputConfig;

	// 해당 Pawn의 Ability System에 허용할 AbilitySet
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LyraClone|Abilities")
	TArray<TObjectPtr<ULyraCloneAbilitySet>> AbilitySets;
};