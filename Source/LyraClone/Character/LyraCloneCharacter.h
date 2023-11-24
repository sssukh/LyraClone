#pragma once

#include "GameFramework/Character.h"
#include "LyraCloneCharacter.generated.h"

UCLASS()
class ALyraCloneCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	ALyraCloneCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};