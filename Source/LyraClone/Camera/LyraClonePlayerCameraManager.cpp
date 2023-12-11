#include "LyraClonePlayerCameraManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraClonePlayerCameraManager)

ALyraClonePlayerCameraManager::ALyraClonePlayerCameraManager(const FObjectInitializer& ObjectInitializer) 
: Super(ObjectInitializer)
{
	DefaultFOV = LyraClone_CAMERA_DEFAULT_FOV;
	ViewPitchMin = LyraClone_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = LyraClone_CAMERA_DEFAULT_PITCH_MAX;
}
