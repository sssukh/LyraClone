#include "LyraCloneCameraComponent.h"
#include "LyraCloneCameraMode.h"

ULyraCloneCameraComponent::ULyraCloneCameraComponent(const FObjectInitializer& ObjectInitializer)
{
}



void ULyraCloneCameraComponent::OnRegister()
{
	Super::OnRegister();
	if (!CameraModeStack)
	{
		// 초기화 (BeginPlay와 같은)가 딱히 필요없는 객체로 NewObject로 할당
		CameraModeStack = NewObject<ULyraCloneCameraModeStack>(this);
	}
}

void ULyraCloneCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	check(CameraModeStack);

	UpdateCameraModes();
}

void ULyraCloneCameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);

	// DeterminCameraModeDelegate는 CameraMode Class를 반환한다.
	// - 해당 delegate는 HeroComponent의 멤버함수로 바인딩되어 있다.
	if (DetermineCameraModeDelegate.IsBound())
	{
		if (const TSubclassOf<ULyraCloneCameraMode> CameraMode = DetermineCameraModeDelegate.Execute())
		{
			// CameraModeStack->PushCameraMode(CameraMode);
		}
	}
}