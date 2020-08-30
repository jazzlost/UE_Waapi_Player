#pragma once

//#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

#include "WaapiTargetObject.h"

class SPlayerTextViewWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SPlayerTextViewWidget)
		: _ShowWidget(true)
		, _WaapiEventObject()
	{}

		SLATE_ARGUMENT(TSharedPtr<FWaapiEventObject>, WaapiEventObject)

		SLATE_ARGUMENT(bool, ShowWidget)

	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

	SPlayerTextViewWidget();
	~SPlayerTextViewWidget();

private:
	//TSharedRef<STextBlock> GenerateTextWidget();

private:
	TSharedPtr<FWaapiEventObject> EventObjectPtr;
	TSharedPtr<class IDetailsView> WaapiTargetDetailsView;
};
