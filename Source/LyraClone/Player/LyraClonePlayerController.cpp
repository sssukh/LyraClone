#include "LyraClonePlayerController.h"
#include "LyraClone/Camera/LyraClonePlayerCameraManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraClonePlayerController)

ALyraClonePlayerController::ALyraClonePlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ALyraClonePlayerCameraManager::StaticClass();
}
