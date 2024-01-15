#pragma once

#include "CoreMinimal.h"
#include "LyraClone/Equipment/LyraCloneGameplayAbility_FromEquipment.h"
#include "LyraCloneGameplayAbility_RangedWeapon.generated.h"

// forward declaration
class ULyraCloneRangedWeaponInstance;

// 어디 대상으로 Target으로 설정할지 옵션들 (Lyra의 경우, 다양한 옵션 존재
UENUM(BlueprintType)
enum class ELyraCloneAbilityTargetingSource : uint8
{
	// Camera 기준으로 진행
	CameraTowardsFocus,
};


UCLASS()
class ULyraCloneGameplayAbility_RangedWeapon : public ULyraCloneGameplayAbility_FromEquipment
{
	GENERATED_BODY()
public:
	struct FRangedWeaponFiringInput
	{
		FVector StartTrace;
		FVector EndAim;
		FVector AimDir;
		ULyraCloneRangedWeaponInstance* WeaponData = nullptr;
		bool bCanPlayBulletFX = false;

		FRangedWeaponFiringInput()
			: StartTrace(ForceInitToZero)
			, EndAim(ForceInitToZero)
			, AimDir(ForceInitToZero)
		{}
	};
	ULyraCloneGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ECollisionChannel DetermineTraceChannel(FCollisionQueryParams& TraceParams, bool bIsSimulated)const;
	void AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams)const;
	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHitResults) const;
	FHitResult DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace, float sweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits) const;
	void TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHits);

	ULyraCloneRangedWeaponInstance* GetWeaponInstance();
	FVector GetWeaponTargetingSourceLocation() const;
	FTransform GetTargetingTransform(APawn* SourcePawn, ELyraCloneAbilityTargetingSource Source);
	void PerformLocalTargeting(TArray<FHitResult>& OutHits);
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);

	UFUNCTION(BlueprintCallable)
	void StartRangedWeaponTargeting();
	
	// Called when target data is ready
	UFUNCTION(BlueprintImplementableEvent)
	void OnRangeWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);
};