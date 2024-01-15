#pragma once

#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "LyraClonePawnExtensionComponent.generated.h"

class ULyraClonePawnData;
class ULyraCloneAbilitySystemComponent;

UCLASS()
class ULyraClonePawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
public:
	ULyraClonePawnExtensionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// feature 정의
	static const FName NAME_ActorFeatureName;

	// member methods

	static ULyraClonePawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<ULyraClonePawnExtensionComponent>() : nullptr); }
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void SetPawnData(const ULyraClonePawnData* InPawnData);
	void SetupPlayerInputComponent();
	void InitializeAbilitySystem(ULyraCloneAbilitySystemComponent* InASC, AActor* InOwnerActor);
	void UninitializeAbilitySystem();
	ULyraCloneAbilitySystemComponent* GetLyraCloneAbilitySystemComponent() const { return AbilitySystemComponent; }

	// UPawnComponent interfaces 
	// 
	// beinplay보다 빠른 시점에 호출된다.
	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

	// IGameFrameworkInitStateInterface
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void CheckDefaultInitialization() final;

	// Pawn을 생성한 데이터를 캐싱
	UPROPERTY(EditInstanceOnly, Category = "LyraClone|Pawn")
	TObjectPtr<const ULyraClonePawnData> PawnData;

	// AbilitySystemComponent 캐싱
	UPROPERTY()
	TObjectPtr<ULyraCloneAbilitySystemComponent> AbilitySystemComponent;
};