#include "SPlayerComboWidget.h"
#include "WaapiPlayerStyle.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SSpinBox.h"
#include "Misc/ScopedSlowTask.h"
//#include "Widgets/Input/SHyperlink.h"

#include "WaapiPlayerAssetManager.h"


#define LOCTEXT_NAMESPACE "WaapiPlayer"


SPlayerSwitchWidget::SPlayerSwitchWidget()
{
	
}


SPlayerSwitchWidget::~SPlayerSwitchWidget()
{

}

void SPlayerSwitchWidget::Construct(const FArguments& InArgs)
{   
	CollectionsComboList = InArgs._OptionList;
	FString DefaultOptionString = CollectionsComboList[0]->ToString();
	if (DefaultOptionString.IsEmpty())
	{
		DefaultOptionString = TEXT("None");
	}

	bIsSwitch = InArgs._IsSwitch;
	FString GroupTitleString = TEXT("Switch Group");
	FString TypeTitleString = TEXT("Switch");
	if (!bIsSwitch)
	{
		GroupTitleString = TEXT("State Group");
		TypeTitleString = TEXT("State");
	}

	ChildSlot
	[
		//SNew(SBorder)
		//.Padding(4)
		//.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
		//[
		//	SNew(SOverlay)

		//	// Picker
		//	+ SOverlay::Slot()
		//	.VAlign(VAlign_Fill)
		//	[
				SNew(SVerticalBox)
				.Visibility(EVisibility::Visible)

				// Separator
				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 0, 1, 1)
				[
					SNew(SSeparator)
					.Visibility(EVisibility::Visible)
				]

				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)

					+SHorizontalBox::Slot()
					.HAlign(EHorizontalAlignment::HAlign_Fill)
					[
						SNew(STextBlock)
						.Text(FText::FromString(GroupTitleString))
					]

					+SHorizontalBox::Slot()
					.HAlign(EHorizontalAlignment::HAlign_Fill)
					[
						SNew(STextBlock)
						.Text(FText::FromString(InArgs._GroupName))
						.ColorAndOpacity(FColor::Yellow)
					]
				]

				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)

					+SHorizontalBox::Slot()
					.HAlign(EHorizontalAlignment::HAlign_Fill)
					[
						SAssignNew(SwitchOrStateTitleText, STextBlock)
						.Text(FText::FromString(TypeTitleString))
					]

					+SHorizontalBox::Slot()
					.HAlign(EHorizontalAlignment::HAlign_Fill)
					[
						SNew(SComboBox<TSharedPtr<FName>>)
						.OptionsSource(&CollectionsComboList)
						.OnGenerateWidget(this, &SPlayerSwitchWidget::OnGenerateWidget)
						.OnSelectionChanged(this, &SPlayerSwitchWidget::OnSelectionChanged)
						//.ToolTipText(this, &SReferenceViewer::GetCollectionFilterText)
						[
							SAssignNew(SelectedSwitchOptionText, STextBlock)
							.Text(FText::FromString(DefaultOptionString))
						]
					]

				]
		//	]
		//]
	];
}

TSharedRef<SWidget> SPlayerSwitchWidget::OnGenerateWidget(TSharedPtr<FName> InItem)
{
	FText ItemAsText = FText::FromName(*InItem);
	return
		SNew(SBox)
		.WidthOverride(300)
		[
			SNew(STextBlock)
			.Text(ItemAsText)
			.ToolTipText(ItemAsText)
		];
}


void SPlayerSwitchWidget::OnSelectionChanged(TSharedPtr<FName> SelectedItems, ESelectInfo::Type Type)
{
	FString OptionString = SelectedItems->ToString();
	SelectedSwitchOptionText->SetText(OptionString);
}
//=================================================================================================================

void SPlayerRtpcWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SNew(SVerticalBox)
			.Visibility(EVisibility::Visible)

			// Separator
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 0, 1, 1)
			[
				SNew(SSeparator)
				.Visibility(EVisibility::Visible)
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				[
				SNew(STextBlock)
				.Text(LOCTEXT("PlayerRtpcTitle", "RTPC Name"))
				]

				+ SHorizontalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				[
					SNew(STextBlock)
					.Text(FText::FromString(InArgs._RtpcName))
				]
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("PlayerRtpcValueTitle", "RTPC Value"))
				]

				+ SHorizontalBox::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				[
					SNew(SSpinBox<float>)
					.MinValue(InArgs._MinValue)
					.MaxValue(InArgs._MaxValue)
					.Value(InArgs._DefaultValue)
					.OnValueChanged(this, &SPlayerRtpcWidget::OnValueChanged)
				]

			]

		];
}

SPlayerRtpcWidget::SPlayerRtpcWidget()
{
}

SPlayerRtpcWidget::~SPlayerRtpcWidget()
{
}

void SPlayerRtpcWidget::OnValueChanged(float Value)
{

}

#undef LOCTEXT_NAMESPACE

