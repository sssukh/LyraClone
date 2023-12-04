#pragma once

#include "LyraCloneCameraMode.h"
#include "LyraCloneCameraMode_ThirdPerson.generated.h"

UCLASS(Abstract, Blueprintable)
class ULyraCloneCameraMode_ThirdPerson : public ULyraCloneCameraMode
{
	GENERATED_BODY()
public:
	ULyraCloneCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};