// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraCloneGameMode.h"

#include "LyraCloneGameState.h"
#include "LyraClone/Character/LyraCloneCharacter.h"
#include "LyraClone/Player/LyraClonePlayerController.h"
#include "LyraClone/Player/LyraClonePlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneGameMode)


ALyraCloneGameMode::ALyraCloneGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GameStateClass = ALyraCloneGameState::StaticClass();
	PlayerControllerClass = ALyraClonePlayerController::StaticClass();
	PlayerStateClass = ALyraClonePlayerState::StaticClass();
	DefaultPawnClass = ALyraCloneCharacter::StaticClass();
}


void ALyraCloneGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// 다음 틱에 등록된 함수를 호출시켜라(델리게이트이다)
	// 아직 GameInstance가 초기화가 완료되지 않았기 때문에 현재 프레임에서 Experience를 처리할 수 없다.
	// 때문에 다음 프레임에서 처리하도록 이렇게 한다.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void ALyraCloneGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
}
