#include "LyraCloneCharacter.h"
#include "LyraClonePawnExtensionComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneCharacter)

ALyraCloneCharacter::ALyraCloneCharacter(const FObjectInitializer& ObjectInitializer)
{
	PawnExtComponent = CreateDefaultSubobject<ULyraClonePawnExtensionComponent>(TEXT("PawnExtensionComponent"));
}

void ALyraCloneCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Pawn이 Possess로서, Controller와 PlayerState 정보 접근이 가능한 상태가 되었음 :
	// - SetupPlayerInputComponent 확인 
	PawnExtComponent->SetupPlayerInputComponent();
}
