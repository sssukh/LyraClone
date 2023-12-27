#include "LyraCloneGameMode.h"
#include "LyraCloneExperienceManagerComponent.h"
#include "LyraCloneGameState.h"
#include "LyraClone/LyraCloneChannels.h"
#include "Kismet/GameplayStatics.h"
#include "LyraClone/Character/LyraCloneCharacter.h"
#include "LyraClone/Player/LyraClonePlayerController.h"
#include "LyraClone/Player/LyraClonePlayerState.h"
#include "LyraClone/Character/LyraClonePawnData.h"
#include "LyraClone/Character/LyraClonePawnExtensionComponent.h"
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
 * InitGame은 맵이 초기화되면서 가장 먼처 호출되는 함수.
 */
void ALyraCloneGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// 아직 GameInstance를 통해, 초기화 작업이 진행되므로, 현 프레임에는 Lyra의 Concept인 Experience 처리를 진행할 수 없다:
	// 이를 처리하기 위해, 한프레임 뒤에 이벤트를 받아 처리를 이어서 진행한다
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void ALyraCloneGameMode::InitGameState()
{
	Super::InitGameState();

	// Experience 비동기 로딩을 위해 Delegatge를 준비한다
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

/*
 * GameMode에서 설정된 Pawn이 소환이 완료되면 호출되는 함수
 */
APawn* ALyraCloneGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) 
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			// FindPawnExtensionComponent 구현
			if (ULyraClonePawnExtensionComponent* PawnExtComp = ULyraClonePawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
			{
				if (const ULyraClonePawnData* PawnData = GetPawnDataForController(NewPlayer))
				{
					PawnExtComp->SetPawnData(PawnData);
				}
			}
			SpawnedPawn->FinishSpawning(SpawnTransform);
			return SpawnedPawn;
		}
	}

	return nullptr;
}

/*
 * Pawn을 소환해주는 함수
 */
void ALyraCloneGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// Experince Loading이 완료되었다면 Pawn 생성
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

void ALyraCloneGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	// 해당 함수에서는 우리가 로딩할 Experience에 대해 PrimaryAssetId를 생성하여, OnMatchAssignmentGiven으로 넘겨준다
	FPrimaryAssetId ExperienceId;

	// precedence order (highest wins)
	// - matchmaking assignment (if present)
	// - default experience

	UWorld* World = GetWorld();

	if (!ExperienceId.IsValid() && UGameplayStatics::HasOption(OptionsString, TEXT("Experience")))
	{
		const FString ExperienceFromOptions = UGameplayStatics::ParseOption(OptionsString, TEXT("Experience"));
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType(ULyraCloneExperienceDefinition::StaticClass()->GetFName()),
			FName(*ExperienceFromOptions));
	}

	// fall back to the default experience
	// 일단 기본 옵션으로 default하게 B_LyraCloneDefaultExperience로 설정놓자
	if (!ExperienceId.IsValid())
	{
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("LyraCloneExperienceDefinition"), FName("B_LyraCloneDefaultExperience"));
	}

	// 필자가 이해한 HandleMatchAssignmentIfNotExpectingOne과 OnMatchAssignmentGiven()은 아직 직관적으로 이름이 와닫지 않는다고 생각한다
	// - 후일, 어느정도 Lyra가 구현되면, 해당 함수의 명을 더 이해할 수 있을 것으로 예상한다
	OnMatchAssignmentGiven(ExperienceId);
}

/*
 *  Expericne 로딩이 되었는지 확인하는 함수
 */
bool ALyraCloneGameMode::IsExperienceLoaded() const
{
	check(GameState);
	const ULyraCloneExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraCloneExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	return ExperienceManagerComponent->IsExperienceLoaded();
}

void ALyraCloneGameMode::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId)
{
	// 해당 함수는 ExperienceManagerComponent을 활용하여 Experience을 로딩하기 위해, ExperienceManagerComponent의 ServerSetCurrentExperience를 호출한다

	check(ExperienceId.IsValid());

	ULyraCloneExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<ULyraCloneExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->ServerSetCurrentExperience(ExperienceId);
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
	// 아직 PlayerState에 PawnData가 설정되어 있지 않은 경우, ExperienceManagerComponent의 CurrentExperience로부터 가져와서 설정
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


/*
 * 로딩이 완료 된 후 Expericne를 받아와서 초기화 해주기 위한 함수
 */
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

