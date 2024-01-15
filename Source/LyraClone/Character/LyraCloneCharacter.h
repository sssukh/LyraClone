#pragma once

#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "LyraCloneCharacter.generated.h"

class ULyraClonePawnExtensionComponent;
class ULyraCloneCameraComponent;

UCLASS()
class ALyraCloneCharacter : public AModularCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ALyraCloneCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// ACharacter interface
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) final;

	// IAbiltySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="LyraClone|Character")
	TObjectPtr<ULyraClonePawnExtensionComponent> PawnExtComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LyraClone|Character")
	TObjectPtr<ULyraCloneCameraComponent> CameraComponent;
};