#include "LyraCloneHeroComponent.h"
#include "LyraClonePawnData.h"
#include "LyraClonePawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "LyraClone/LyraCloneChannels.h"
#include "LyraClone/LyraCloneGameplayTags.h"
#include "LyraClone/Camera/LyraCloneCameraComponent.h"
#include "LyraCLone/Player/LyraCLonePlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneHeroComponent)

// FeatureName 정의 " static member variable 초기화
const FName ULyraCloneHeroComponent::NAME_ActorFeatureName("Hero");

ULyraCloneHeroComponent::ULyraCloneHeroComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// LyraCloneHeroComponent도 PawnExtensionComponent와 동일하게 초기화를 이벤트로 처리할 예정이므로 Tick을 끕니다.
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

TSubclassOf<ULyraCloneCameraMode> ULyraCloneHeroComponent::DeterminCameraMode() const
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (ULyraClonePawnExtensionComponent* PawnExtComp = ULyraClonePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const ULyraClonePawnData* PawnData = PawnExtComp->GetPawnData<ULyraClonePawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}

void ULyraCloneHeroComponent::OnRegister()
{
	Super::OnRegister();

	// 올바른 Actor에 등록되었는지 확인
	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogLyraClone, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
			return;
		}
	}

	// 고유 식별번호를 ComponentManager에게 등록
	RegisterInitStateFeature();
}

void ULyraCloneHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// PawnExtension의 경우 NAME_None을 통해 모든 컴포넌트의 상태를 확인하였지만, HeroComponent는 PawnExtension만 관찰할 수 있도록 합니다.
	BindOnActorInitStateChanged(ULyraClonePawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// InitState_Spawned로 초기화
	ensure(TryToChangeInitState(FLyraCloneGameplayTags::Get().InitState_Spawned));

	// ForceUpdate 진행
	CheckDefaultInitialization();
}

void ULyraCloneHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void ULyraCloneHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	
	const FLyraCloneGameplayTags& InitTags = FLyraCloneGameplayTags::Get();

	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		// ULyraClonePawnExtensionComponent의 DataInitialized 상태 변화 관찰 후, LyraCloneHeroComponent도 Datainitialized 상태로 변경
		// - CanChangedInitState 확인
		if (Params.FeatureState == InitTags.InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

bool ULyraCloneHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	const FLyraCloneGameplayTags& InitTags = FLyraCloneGameplayTags::Get();
	ALyraClonePlayerState* PS = GetPlayerState<ALyraClonePlayerState>();

	// InitState_Spawned 초기화
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}

	// Spawned -> DataAvailable
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		if (!PS)
		{
			return false;
		}

		return true;
	}

	// DataAvailable->DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// PawnExtensionComponent가 DataInitialized될 떄까지 기다림 (== 모든 Feature Component가 DataAvailable인 상태)
		return PS&&Manager->HasFeatureReachedInitState(Pawn,ULyraClonePawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

// Input과 Camera를 핸들링 하는 함수
void ULyraCloneHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manger, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	const FLyraCloneGameplayTags& InitTags = FLyraCloneGameplayTags::Get();

	// DataAvailable ->DataInitialized 단계
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ALyraClonePlayerState* PS = GetPlayerState<ALyraClonePlayerState>();
		if (!ensure(Pawn && PS))
		{
			return;
		}
		// Input과 Camera에 대한 핸들링 (TODO)

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const ULyraClonePawnData* PawnData = nullptr;
		if (ULyraClonePawnExtensionComponent* PawnExtComp = ULyraClonePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<ULyraClonePawnData>();
		}
		
		if (bIsLocallyControlled && PawnData)
		{
			// 현재 LyraCloneCharacter에 Attach된 CameraComponent를 찾음
			if (ULyraCloneCameraComponent* CameraComponent = ULyraCloneCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DeterminCameraMode);
			}
		}
	}

	
}

void ULyraCloneHeroComponent::CheckDefaultInitialization()
{
	// 앞서 BindOnActorInitStateChanged에서 보았듯이 Hero Feature는 Pawn Extension Feature에 종속되어 있으므로, CheckDefaultInitializeationForImplementers 호출하지 않음 :

	// ContinueInitStateChain은 앞서 PawnExtComponent와 같음
	const FLyraCloneGameplayTags& InitTags = FLyraCloneGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	ContinueInitStateChain(StateChain);
}
