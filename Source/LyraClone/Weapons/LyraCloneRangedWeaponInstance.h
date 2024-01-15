#pragma once
#include "LyraCloneWeaponInstance.h"
#include "LyraCloneRangedWeaponInstance.generated.h"
UCLASS()
class ULyraCloneRangedWeaponInstance : public ULyraCloneWeaponInstance
{
	GENERATED_BODY()
public:
	ULyraCloneRangedWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 유효 사거리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponConfig", meta = (ForceUnits = cm))
	float MaxDamageRange = 25000.0f;

	// 총탄 범위 (Sphere Trace Sweep)
	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = "WeaponConfig", meta =(ForceUnits = cm))
	float BulletTraceWeaponRadius = 0.0f;
};