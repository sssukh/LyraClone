#pragma once

#include "Camera/PlayerCameraManager.h"
#include "LyraClonePlayerCameraManager.generated.h"

// Controller에 바인딩된 CameraManager
#define LyraClone_CAMERA_DEFAULT_FOV (80.0f)
#define LyraClone_CAMERA_DEFAULT_PITCH_MIN (-89.0f)
#define LyraClone_CAMERA_DEFAULT_PITCH_MAX (89.0f)

UCLASS()
class ALyraClonePlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	ALyraClonePlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};