#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UIExtensionSystem.h"
#include "Components/DynamicEntryBoxBase.h"
#include "UIExtensionPointWidget.generated.h"

class UCommonLocalPlayer;

/**
* UUIExtensionPointWidget은 UIExtension 붙을 위젯의 하나 단위로 생각하면 된다:
* - UIExtension에 결합된 Context별로 관리하는 객체가 UIExtensionPoint(Handle)이다:
* - 예를 들어, 해당 UIExtension Slot에 PlayerState/PlayerController/LocalPlayer와 같이 다양한 Context로 부터 Widget이 결합될 수 있다.
*	이를 분리하며 관리하기 위해 만들어진 객체가 UIExtensionPoint이다.
*/

UCLASS()
class UIEXTENSION_API UUIExtensionPointWidget : public UDynamicEntryBoxBase
{
	GENERATED_BODY()
public:
	UUIExtensionPointWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void ResetExtensionPoint();
	void RegisterExtensionPoint();
	void RegisterExtensionPointForPlayerState(UCommonLocalPlayer* LocalPlayer, APlayerState* PlayerState);
	void OnAddOrRemoveExtension(EUIExtensionAction Action, const FUIExtensionRequest& Request);

	// UWidget's interfaces
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
	FGameplayTag ExtensionPointTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
	EUIExtensionPointMatch ExtensionPointTagMatch = EUIExtensionPointMatch::ExactMatch;

	// UIExtensionPointWidget에 결합이 허용된 UWidget Classes
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Extension")
	TArray<TObjectPtr<UClass>> DataClasses;

	// UIExtension --- Widget 관계 매핑
	UPROPERTY(Transient)
	TMap<FUIExtensionHandle, TObjectPtr<UUserWidget>> ExtensionMapping;

	// UIExtensionPoint 객체 관리
	TArray<FUIExtensionPointHandle> ExtensionPointHandles;
};
