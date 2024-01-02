#pragma once
#include "LyraCloneCharacterPartTypes.h"
#include "Components/ControllerComponent.h"
#include "LyraCloneControllerComponent_CharacterParts.generated.h"

class ULyraClonePawnComponent_CharacterParts;

// ControllerComponent가 소유하는 Character Parts
USTRUCT()
struct FLyraCloneControllerCharacterPartEntry
{
	GENERATED_BODY()
	
	// Character Part에 대한 정의(메타 데이터 == MetaData)
	UPROPERTY(EditAnywhere)
	FLyraCloneCharacterPart Part;

	// Character Part 핸들 (고유값) - Controller가 Possess하고 있는 Pawn에서 생성한 (인스턴스) Character Part 핸들값
	FLyraCloneCharacterPartHandle Handle;
};

// Controller가 Pawn을 Possess했을 때, Pawn에 어떤 Cosmetic Actor 생성할지 결정하는 ControllerComponent:
// - 필자는 캐릭터 파츠를 유저 관점에서 관리하는 Component로 이해한다.
UCLASS(meta = (BlueprintSpawnableComponent))
class ULyraCloneControllerComponent_CharacterParts : public UControllerComponent
{
	GENERATED_BODY()
public:
	ULyraCloneControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	ULyraClonePawnComponent_CharacterParts* GetPawnCustomizer() const;

	UFUNCTION(BlueprintCallable, Category = Cosmetics)
	void AddCharacterPart(const FLyraCloneCharacterPart& NewPart);

	void AddCharacterPartInternal(const FLyraCloneCharacterPart& NewPart);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void RemoveAllCharacterParts();

	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FLyraCloneControllerCharacterPartEntry> CharacterParts;


};
