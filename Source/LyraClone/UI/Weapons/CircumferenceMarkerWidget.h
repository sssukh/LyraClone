#pragma once

#include "Components/Widget.h"
#include "SCircumferenceMarkerWidget.h"
#include "CircumferenceMarkerWidget.generated.h"

// Blueprint 상에서 데이터가 여기에 들어오고
// 이 데이터를 SWidget에 전달한다.

UCLASS()
class UCircumferenceMarkerWidget : public UWidget
{
	GENERATED_BODY()
public:
	UCircumferenceMarkerWidget(const FObjectInitializer& ObjectInitializer);

	// UWidget interfaces

	// SWidget과 UWidget간 데이터 싱크를 맞추기 위한 메서드
	virtual void SynchronizeProperties() override;
	//Widget 재생성
	virtual TSharedRef<SWidget> RebuildWidget() override;

	// UVisual interface

	// 해당 UWidget은 MyMarkerWidget의 SWidget을 가지고 있으므로 잘 해제해야 한다.
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	TArray<FCircumferenceMarkerEntry> MarkerList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	float Radius = 48.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FSlateBrush MarkerImage;

	UPROPERTY(EditAnywhere, Category = Corner)
	uint8 bReticleCornerOutsideSpreadRadius : 1;

	// UMG의 CircumferenceMarkerWidget에 대응되는 SWidget
	TSharedPtr<SCircumferenceMarkerWidget> MyMarkerWidget;
};