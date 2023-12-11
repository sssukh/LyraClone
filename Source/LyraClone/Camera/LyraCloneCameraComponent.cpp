#include "LyraCloneCameraComponent.h"
#include "LyraCloneCameraMode.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneCameraComponent)

ULyraCloneCameraComponent::ULyraCloneCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CameraModeStack(nullptr)
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

void ULyraCloneCameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);

	// DeterminCameraModeDelegate는 CameraMode Class를 반환한다.
	// - 해당 delegate는 HeroComponent의 멤버함수로 바인딩되어 있다.
	if (DetermineCameraModeDelegate.IsBound())
	{
		if (TSubclassOf<ULyraCloneCameraMode> CameraMode = DetermineCameraModeDelegate.Execute())
		{
			CameraModeStack->PushCameraMode(CameraMode);
		}
	}
}

void ULyraCloneCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	check(CameraModeStack);

	UpdateCameraModes();

	FLyraCloneCameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, CameraModeView);

	if (APawn* TargetPawn = Cast<APawn>(GetTargetActor()))
	{
		if (APlayerController* PC = TargetPawn->GetController<APlayerController>())
		{
			// PlayerController의 ControlRotation을 계산된 CameraModeView의 ControlRotation으로 업데이트 해주자
			// - SetControlRotation을 외우기보다 한번 코드를 보자 :
			// - 해당 함수는 PC가 Possess하고 있는 Pawn의 RootComponent에 ControlRotation을 반영한다 (== 조정하고 있는 캐릭터에 회전?)
			PC->SetControlRotation(CameraModeView.ControlRotation);
		}
	}

	// Camera의 Location과 Rotation을 반영하자 :
	// - 시간 있으면, SetWorldLocationAndRotation을 보면서 -> UpdateChildTransform까지 보는 것을 추천
	// - SceneGraph 관계 업데이트를 이해할 수 있다
	// 간단하게 정리하면, CameraComponent에 대해 Parent의 SceneGraph 관계를 고려하여 업데이트 진행한다.
	SetWorldLocationAndRotation(CameraModeView.Location, CameraModeView.Rotation);

	// FOV 업데이트
	FieldOfView = CameraModeView.FieldOfView;

	/**
	* 여기서 우리는 ControlRotation vs Rotation의 차이점을 이해할 수 있다 :
	*  - ControlRotation은 PC가 조정하는 Pawn의 Rotation을 적용할 값이다
	*  - 그에 반해, Rotation은 Camera에 반영하는 Rotation이라는 것이다.
	*/

	// FMinimalViewInfo를 업데이트 해준다 :
	// - CameraComponent의 변화 사항을 반영해서 최종 렌더링까지 반영하게 됨
	DesiredView.Location = CameraModeView.Location;
	DesiredView.Rotation = CameraModeView.Rotation;
	DesiredView.FOV = CameraModeView.FieldOfView;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	DesiredView.ProjectionMode = ProjectionMode;
	DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;
	if (PostProcessBlendWeight > 0.0f)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}

}

