#pragma once
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Delegates/Delegate.h"
#include "AsyncAction_ExperienceReady.generated.h"

// BP를 통한 UFunction으로 바인딩하는 Multicast Delegate : Dynamic!
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExperienceReadyAsyncDelegate);

class AGameStateBase;
class ULyraCloneExperienceDefinition;

UCLASS()
class UAsyncAction_ExperienceReady : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// UAsyncAction_ExperienceReady를 생성하고 기다리는 BP 호출
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static UAsyncAction_ExperienceReady* WaitForExperienceReady(UObject* WorldContextObject);

	// UBlueprintAsyncActionBase
	virtual void Activate() override;

	void Step1_HandleGameStateSet(AGameStateBase* GameState);
	void Step2_ListenToExperienceLoading(AGameStateBase* GameState);
	void Step3_HandleExperienceLoaded(const ULyraCloneExperienceDefinition* CurrentExperience);
	void Step4_BroadcastReady();

	// BlueprintAssignable은 BP상에서 할당 가능ㅎ나 변수로 정의한다
	UPROPERTY(BlueprintAssignable)
	FExperienceReadyAsyncDelegate OnReady;

	// WorldPtr을 단순 로직상 캐싱하는 용도
	TWeakObjectPtr<UWorld> WorldPtr;
};