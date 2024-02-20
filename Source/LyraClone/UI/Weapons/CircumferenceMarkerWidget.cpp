#include "CircumferenceMarkerWidget.h"
#include "SCircumferenceMarkerWidget.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(CircumferenceMarkerWidget)

UCircumferenceMarkerWidget::UCircumferenceMarkerWidget(const FObjectInitializer& Objectinitializer)
	:Super(Objectinitializer)
{
}

void UCircumferenceMarkerWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyMarkerWidget.Reset();
}

void UCircumferenceMarkerWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	MyMarkerWidget->SetRadius(Radius);
	MyMarkerWidget->SetMarkerList(MarkerList);
}

TSharedRef<SWidget> UCircumferenceMarkerWidget::RebuildWidget()
{
	MyMarkerWidget = SNew(SCircumferenceMarkerWidget)
		.MarkerBrush(&MarkerImage)
		.Radius(this->Radius)
		.MarkerList(this->MarkerList);
	return MyMarkerWidget.ToSharedRef();
}