#include "SPlayerTextViewWidget.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"



SPlayerTextViewWidget::SPlayerTextViewWidget()
{
}

SPlayerTextViewWidget::~SPlayerTextViewWidget()
{
}


void SPlayerTextViewWidget::Construct(const FArguments & InArgs)
{
	EventObjectPtr = InArgs._WaapiEventObject;

	TSharedPtr<SScrollBox> TempScroll;

	const bool bIsUpdatable = false;
	const bool bAllowFavorites = true;
	const bool bIsLockable = false;

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	for (int i = 0; i < EventObjectPtr->Targets.Num(); i++)
	{
		FDetailsViewArgs DetailsViewArgs(bIsUpdatable, bIsLockable, true, FDetailsViewArgs::ObjectsUseNameArea, false);
		TSharedPtr<class IDetailsView> WaapiTargetDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
		WaapiTargetDetailsView->SetObject(EventObjectPtr->Targets[i]);

		WaapiTargetDetailsViews.Add(WaapiTargetDetailsView);
	}


	ChildSlot
	[
		SNew(SVerticalBox)

		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString(EventObjectPtr->EventName))
		]

		+ SVerticalBox::Slot()
		.FillHeight(80)
		[
			SAssignNew(TempScroll, SScrollBox)
			.Orientation(EOrientation::Orient_Vertical)
			.ScrollBarAlwaysVisible(true)
		]
	];

	for (int i = 0; i < EventObjectPtr->Targets.Num(); i++)
	{
		TempScroll->AddSlot()
			[
				WaapiTargetDetailsViews[i].ToSharedRef()
			];
	};
}

