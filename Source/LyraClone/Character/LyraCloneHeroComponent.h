#pragma once

#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "LyraCloneHeroComponent.generated.h"

struct FInputActionValue;
struct FLyraCloneMappableConfigPair;
class ULyraCloneCameraMode;

UCLASS(Blueprintable,Meta=(BlueprintSpawnableComponent))
class ULyraCloneHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
public:
	ULyraCloneHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	// FeatureName 정의
	static const FName NAME_ActorFeatureName;

	// UPawnComponent interface
	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

	// IGameFrameworkInitStateInterface
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manger, FGameplayTag CurrentState, FGameplayTag DesiredState)	 final;
	virtual void CheckDefaultInitialization() final;

	// member methods
	TSubclassOf<ULyraCloneCameraMode> DeterminCameraMode() const;
	void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	static const FName NAME_BindInputsNow;


	UPROPERTY(EditAnywhere)
	TArray<FLyraCloneMappableConfigPair> DefaultInputConfigs;
};