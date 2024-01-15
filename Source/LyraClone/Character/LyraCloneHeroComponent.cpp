#include "LyraCloneHeroComponent.h"
#include "EnhancedInputSubsystems.h"
#include "LyraClonePawnData.h"
#include "LyraClonePawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "LyraClone/LyraCloneChannels.h"
#include "LyraClone/LyraCloneGameplayTags.h"
#include "LyraClone/Camera/LyraCloneCameraComponent.h"
#include "LyraClone/Input/LyraCloneInputComponent.h"
#include "LyraClone/Input/LyraCloneMappableConfigPair.h"
#include "LyraClone/Player/LyraClonePlayerController.h"
#include "LyraClone/Player/LyraCLonePlayerState.h"
#include "PlayerMappableInputConfig.h"
#include "LyraClone/AbilitySystem/LyraCloneAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCloneHeroComponent)

// FeatureName 정의 " static member variable 초기화
const FName ULyraCloneHeroComponent::NAME_ActorFeatureName("Hero");

const FName ULyraCloneHeroComponent::NAME_BindInputsNow("BindInputsNow");

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

void ULyraCloneHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	// LocalPlayer를 가져오기 위해
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	// EnhancedInputLocalPlayerSubsystem 가져오기 위해
	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP -> GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	// EnhancedInputLocalPlayerSubsystem에 mappingContext를 비워준다
	Subsystem->ClearAllMappings();

	// PawnExtensionComponent -> PawnData -> InputConfig 존재 유무 판단
	if (const ULyraClonePawnExtensionComponent* PawnExtComp =
		ULyraClonePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const ULyraClonePawnData* PawnData = PawnExtComp->GetPawnData<ULyraClonePawnData>())
		{
			if (const ULyraCloneInputConfig* InputConfig = PawnData->InputConfig)
			{
				const FLyraCloneGameplayTags& GameplayTags = FLyraCloneGameplayTags::Get();

				// HeroComponent 가지고 있는 Input Mapping Context를 순회하며, EnhancedInputLocalPlayerSubsystem에 추가한다
				for (const FLyraCloneMappableConfigPair& Pair : DefaultInputConfigs)
				{
					if (Pair.bShouldActivateAutomatically)
					{
						FModifyContextOptions Options = {};
						Options.bIgnoreAllPressedKeysUntilRelease = false;

						// 내부적으로 Input Mapping Context를 추가한다 :
						// - AddPlayerMappableConfig를 간단히 보는 것을 추천
						Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);
					}
				}
				ULyraCloneInputComponent* LyraCloneIC = CastChecked<ULyraCloneInputComponent>(PlayerInputComponent);
				{
					// InputTag_Move와 InputTag_Look_Mouse에 대해 각각 Input_Move()와 Input_LookMouse() 멤버 함수에 바인딩 시킨다.
					// - 바인딩한 이후, Input 이벤트에 따라 멤버 함수가 트리거된다.
					{
						TArray<uint32> BindHandles;
						LyraCloneIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, BindHandles);
					}
					LyraCloneIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false);
					LyraCloneIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, false);
				}
			}
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);


}

void ULyraCloneHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (Controller)
	{
		
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}
		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}

		bool bLogging = true;
		if (bLogging)
		{
			UE_LOG(LogLyraClone, Log, TEXT("Input_Move[X=%.2f][Y=%.2f]"), Value.X, Value.Y);
		}
	}
}

void ULyraCloneHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}
	const FVector2D Value = InputActionValue.Get<FVector2D>();
	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}
	if (Value.Y != 0.0f)
	{
		double AimInversionValue = -Value.Y;
		Pawn->AddControllerPitchInput(AimInversionValue);
	}
}

void ULyraCloneHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const ULyraClonePawnExtensionComponent* PawnExtComp = ULyraClonePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (ULyraCloneAbilitySystemComponent* LyraCloneASC = PawnExtComp->GetLyraCloneAbilitySystemComponent())
			{
				LyraCloneASC->AbilityInputTagPressed(InputTag);
			}
		}
	}
}

void ULyraCloneHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const ULyraClonePawnExtensionComponent* PawnExtComp = ULyraClonePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (ULyraCloneAbilitySystemComponent* LyraCloneASC = PawnExtComp->GetLyraCloneAbilitySystemComponent())
			{
				LyraCloneASC->AbilityInputTagReleased(InputTag);
			}
		}
	}
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

	if (Params.FeatureName != ULyraClonePawnExtensionComponent::NAME_ActorFeatureName)
	{
		// ULyraClonePawnExtensionComponent의 DataInitialized 상태 변화 관찰 후, LyraCloneHeroComponent도 Datainitialized 상태로 변경
		// - CanChangedInitState 확인
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

bool ULyraCloneHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	const FLyraCloneGameplayTags& InitTags = FLyraCloneGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
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

			// DataInitialized 단계까지 오면, Pawn이 Controller에 Possess되어 준비된 상태이다:
			// - InitAbilityActorInfo 호출로 AvatarActor 재설정이 필요하다.
			PawnExtComp->InitializeAbilitySystem(PS->GetLyraCloneAbilitySystemComponent(), PS);
		}
		
		if (bIsLocallyControlled && PawnData)
		{
			// 현재 LyraCloneCharacter에 Attach된 CameraComponent를 찾음
			if (ULyraCloneCameraComponent* CameraComponent = ULyraCloneCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DeterminCameraMode);
			}
		}

		if (ALyraClonePlayerController* HakPC = GetController<ALyraClonePlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
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
