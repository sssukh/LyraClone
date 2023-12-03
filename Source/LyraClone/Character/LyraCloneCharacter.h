#pragma once

#include "GameFramework/Character.h"
#include "LyraCloneCharacter.generated.h"

class ULyraClonePawnExtensionComponent;

UCLASS()
class ALyraCloneCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	ALyraCloneCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// ACharacter interface
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) final;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="LyraClone|Character")
	TObjectPtr<ULyraClonePawnExtensionComponent> PawnExtComponent;
};