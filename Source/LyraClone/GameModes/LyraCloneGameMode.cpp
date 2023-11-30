// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraCloneGameMode.h"

#include "LyraCloneExperienceManagerComponent.h"
#include "LyraCloneGameState.h"
#include "LyraClone/LyraCloneChannels.h"
#include "LyraClone/Character/LyraCloneCharacter.h"
#include "LyraClone/Player/LyraClonePlayerController.h"
#include "LyraClone/Player/LyraClonePlayerState.h"
#include "LyraClone/Character/LyraClonePawnData.h"
#include "LyraClone/GameModes/LyraCloneExperienceDefinition.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneGameMode)

ALyraCloneGameMode::ALyraCloneGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GameStateClass = ALyraCloneGameState::StaticClass();
	PlayerControllerClass = ALyraClonePlayerController::StaticClass();
	PlayerStateClass = ALyraClonePlayerState::StaticClass();
	DefaultPawnClass = ALyraCloneCharacter::StaticClass();
}

/*
* InitGame은 맵이 초기화되면서 가장 먼저 호출되는 함수
*/

void ALyraCloneGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// 다음 틱에 등록된 함수를 호출시켜라(델리게이트이다)
	// 아직 GameInstance가 초기화가 완료되지 않았기 때문에 현재 프레임에서 Experience를 처리할 수 없다.
	// 때문에 다음 프레임에서 처리하도록 이렇게 한다.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void ALyraCloneGameMode::InitGameState()
{
	Super::InitGameState();

	// Experience 비동기 로딩을 위해 Delegate를 준비한다.
	ULyraCloneExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraCloneExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// OnExperienceLoaded 등록
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnLyraCloneExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

UClass* ALyraCloneGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	// GetPawnDataForController를 활용하여, PawnData로부터 PawnClass를 유도하자
	if (const ULyraClonePawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

// GameMode에서 설정된 Pawn이 소환이 완료되면 호출되는 함수
APawn* ALyraCloneGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	UE_LOG(LogLyraClone, Log, TEXT("SpanwDefaultPawnAtTransform_Implementation is called!"));
	return Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
}

// Pawn을 소환해주는 함수
void ALyraCloneGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// Experience Loading이 완료되었다면 Pawn 생성
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}



void ALyraCloneGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	// 해당 함수에서는 우리가 로딩할 Experience에 대해 PrimaryAssetId를 생성하여, OnMatchAssignmentGiven으로 넘겨준다.
	FPrimaryAssetId ExperienceId;
	
	// precedence order (highest wins)
	// - matchmaking assignment (if present)
	// - default experience
	UWorld* World = GetWorld();


	// fall back to the default experience
	// 일단 기본 옵션으로 default하게 B_LyraCloneDefaultExperience로 설정 놓자
	if (!ExperienceId.IsValid())
	{
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("LyraCloneExperienceDefinition"), FName("B_LyraCloneDefaultExperience"));
	}

	//
	OnMatchAssignmentGiven(ExperienceId);
}


// Experience 로딩이 되었는지 확인하는 함수
bool ALyraCloneGameMode::IsExperienceLoaded() const
{
	check(GameState);
	const ULyraCloneExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraCloneExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	return ExperienceManagerComponent->IsExperienceLoaded();
}

void ALyraCloneGameMode::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId)
{
	// 해당 함수는 ExperienceManagerComponent를 활용하여 Experience를 로딩하기 위해, ExperienceManagerComponent의 ServerSetCurrentExperience를 호출한다.
	check(ExperienceId.IsValid());

	ULyraCloneExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraCloneExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->ServerSetCurrentExperience(ExperienceId);
}

// 로딩이 완료 된 후 Experience를 받아와서 초기화 해주기 위한 함수
void ALyraCloneGameMode::OnExperienceLoaded(const ULyraCloneExperienceDefinition* CurrentExperience)
{
	// PlayerController를 순회하며
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);

		// PlayerController가 Pawn을 Possess하지 않았다면, RestartPlayer를 통해 Pawn을 다시 Spawn한다
		// - 한번 OnPossess를 보도록 하자:
		if (PC && PC->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

const ULyraClonePawnData* ALyraCloneGameMode::GetPawnDataForController(const AController* InController) const
{
	// 게임 도중에 PawnData가 오버라이드 되었을 경우, PawnData는 PlayerState에서 가져오게 됨
	if (InController)
	{
		if (const ALyraClonePlayerState* PS = InController->GetPlayerState<ALyraClonePlayerState>())
		{
			// GetPawnData 구현
			if (const ULyraClonePawnData* PawnData = PS->GetPawnData<ULyraClonePawnData>())
			{
				return PawnData;
			}
		}
	}


	// fall back to the default for the current experience
	// 아직 PlayerState에 PawnData가 설정되어 있지 않은 경우, ExperienceManagerComponent의 CurrentExperience로 부터 가져와서 설정
	check(GameState);
	ULyraCloneExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraCloneExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	if (ExperienceManagerComponent->IsExperienceLoaded())
	{
		// GetExperienceChecked 구현
		const ULyraCloneExperienceDefinition* Experience = ExperienceManagerComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData)
		{
			return Experience->DefaultPawnData;
		}
	}

	// 어떠한 케이스에도 핸들링 안되었으면 nullptr
	return nullptr;
}

