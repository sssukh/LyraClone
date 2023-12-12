#pragma once
#include "LyraCloneMappableConfigPair.generated.h"

class UPlayerMappableInputConfig;

USTRUCT()
struct FLyraCloneMappableConfigPair
{
	GENERATED_BODY()	
public:
	FLyraCloneMappableConfigPair() = default;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;
};