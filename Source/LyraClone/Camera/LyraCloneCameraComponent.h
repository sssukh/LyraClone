#pragma once

#include "Camera/CameraComponent.h"
#include "LyraCloneCameraComponent.generated.h"

// forward declaration
class ULyraCloneCameraModeStack;
class ULyraCloneCameraMode;

// template forward declaration
template <class TClass> class TSubclassOf;

DECLARE_DELEGATE_RetVal(TSubclassOf<ULyraCloneCameraMode>, FLyraCloneCameraModeDelegate);

UCLASS()
class ULyraCloneCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
public:
	ULyraCloneCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static ULyraCloneCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<ULyraCloneCameraComponent>() : nullptr); }

	// member function
	AActor* GetTargetActor() const { return GetOwner(); }
	void UpdateCameraModes();

	// CameraComponent interface
	virtual void OnRegister() final;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) final;

	// member variables

	// 카메라의 Blending 기능을 지원하는 stack
	UPROPERTY()
	TObjectPtr<ULyraCloneCameraModeStack> CameraModeStack;

	// 현재 CameraMode를 가져오는 Delegate
	FLyraCloneCameraModeDelegate DetermineCameraModeDelegate;
};