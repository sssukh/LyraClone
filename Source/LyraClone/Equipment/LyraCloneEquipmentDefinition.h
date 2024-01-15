#pragma once
#include "Containers/Array.h"
#include "Math/Transform.h"
#include "Templates/SubclassOf.h"
#include "UObject/NameTypes.h"
#include "UObject/Object.h"
#include "LyraCloneEquipmentDefinition.generated.h"

class ULyraCloneEquipmentInstance;
class ULyraCloneAbilitySet;

USTRUCT()
struct FLyraCloneEquipmentActorToSpawn
{
	GENERATED_BODY()
	
	// Spawn할 대상 Actor 클래스 (== Actor를 상속받은 Asset으로 생각해도 됨
	UPROPERTY(EditAnywhere, Category = Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	// 어느 Bone Socket에 붙일지 결정한다.
	UPROPERTY(EditAnywhere, Category = Equipment)
	FName AttachSocket;

	// Socket에서 어느정도 Transformation을 더할 것인지 결정 : (Rotation, Position, Scale)
	UPROPERTY(EditAnywhere, Category = Equipment)
	FTransform AttachTransform;
};

/**
* BlueprintType과 Blueprintable은 각각 무엇일까?
* - Blueprintable은 **해당 클래스를 상속받는 모든 클래스는** Blueprint로 정의할 수 있다.
* - BLueprintType은 **BP에서 해당 클래스를 변수**로서 사용 가능하다
**/

// LyraCloneEquipmentDefinition은 장착 아이템에 대한 정의 클래스(메타 데이터)이다.
UCLASS(BlueprintType, Blueprintable)
class ULyraCloneEquipmentDefinition : public UObject
{
	GENERATED_BODY()
public:
	ULyraCloneEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 해당 메타 데이터를 사용하면, 어떤 인스턴스를 Spawn할지 결정하는 클래스
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TSubclassOf<ULyraCloneEquipmentInstance> InstanceType;

	// 해당 장착 아이템을 사용하면, 어떤 Actor가 Spawn이 되는지 정보를 담고 있다.
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<FLyraCloneEquipmentActorToSpawn> ActorsToSpawn;

	// 장착을 통해 부여 가능한 Ability Set
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<TObjectPtr<ULyraCloneAbilitySet>> AbilitySetsToGrant;
};