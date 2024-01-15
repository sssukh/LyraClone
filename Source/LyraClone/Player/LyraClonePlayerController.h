#pragma once

#include "ModularPlayerController.h"
#include "LyraClonePlayerController.generated.h"

class ALyraClonePlayerState;
class ULyraCloneAbilitySystemComponent;

UCLASS()
class ALyraClonePlayerController : public AModularPlayerController
{
	GENERATED_BODY()
public:
	ALyraClonePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// PlayerController interface
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	// member methods
	ALyraClonePlayerState* GetLyraClonePlayerState() const;
	ULyraCloneAbilitySystemComponent* GetLyraCloneAbilitySystemComponent() const;
};