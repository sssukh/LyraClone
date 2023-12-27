#pragma once
#include "CoreMinimal.h"
#include "GameFeatureAction_WorldActionBase.h"
#include "LyraClone/Input/LyraCloneMappableConfigPair.h"

#include "GameFeatureAction_AddInputConfig.generated.h"

struct FComponentRequestHandle;
class APawn;

UCLASS()
class UGameFeatureAction_AddInputConfig : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()
public:
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;
private:
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
		TArray<TWeakObjectPtr<APawn>> PawnsAddedTo;
	};

	void HandlePawnExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext
		ChangeContext);
	void AddInputConfig(APawn* Pawn, FPerContextData& ActiveData);
	void RemoveInputConfig(APawn* Pawn, FPerContextData& ActiveData);
	void Reset(FPerContextData& ActiveData);

	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

	UPROPERTY(EditAnywhere)
	TArray<FLyraCloneMappableConfigPair> InputConfigs;
};
