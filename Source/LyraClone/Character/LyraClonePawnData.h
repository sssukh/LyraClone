#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraClonePawnData.generated.h"


// 사용할 폰이 가질 최소한의 것을 정의하는 곳
UCLASS(BlueprintType)
class ULyraClonePawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	ULyraClonePawnData(const FObjectInitializer& ObjectInitializer);
};