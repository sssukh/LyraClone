#pragma once

#include "LyraCloneCameraMode.generated.h"

// Camera Blending 대상 유닛
UCLASS(Abstract, NotBlueprintable)
class ULyraCloneCameraMode : public UObject
{
	GENERATED_BODY()
public:
	ULyraCloneCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

UCLASS()
class ULyraCloneCameraModeStack : public UObject
{
	GENERATED_BODY()
public:
	ULyraCloneCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 생성된 CameraMode를 관리
	UPROPERTY()
	TArray<TObjectPtr<ULyraCloneCameraMode>> CameraModeInstances;

	// Camera Matrix Blend 업데이트 진행 큐
	TArray<TObjectPtr<ULyraCloneCameraMode>> CameraModeStack;
	
};
