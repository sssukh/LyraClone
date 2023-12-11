#include "LyraCloneCharacter.h"
#include "LyraClonePawnExtensionComponent.h"
#include "LyraClone/Camera/LyraCloneCameraComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneCharacter)

ALyraCloneCharacter::ALyraCloneCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Tick 비활성화
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	// PawnExtenstionComponent 생성
	PawnExtComponent = CreateDefaultSubobject<ULyraClonePawnExtensionComponent>(TEXT("PawnExtensionComponent"));

	// CameraComponent 생성
	{
		CameraComponent = CreateDefaultSubobject<ULyraCloneCameraComponent>(TEXT("CameraComponent"));
		CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
	}
}

void ALyraCloneCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Pawn이 Possess로서, Controller와 PlayerState 정보 접근이 가능한 상태가 되었음 :
	// - SetupPlayerInputComponent 확인 
	PawnExtComponent->SetupPlayerInputComponent();
}
