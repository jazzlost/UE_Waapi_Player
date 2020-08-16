// Copyright (c) 2006-2012 Audiokinetic Inc. / All Rights Reserved

/*------------------------------------------------------------------------------------
	SWwisePicker.cpp
------------------------------------------------------------------------------------*/

#include "SPlayerListViewWidget.h"
//#include "AkAudioDevice.h"
//#include "AkAudioBankGenerationHelpers.h"
//#include "AkAudioStyle.h"
#include "WaapiPlayerStyle.h"
//#include "WwisePicker/SWwisePicker.h"
//#include "WwisePicker/WwiseWwuParser.h"
//#include "DirectoryWatcherModule.h"
//#include "IDirectoryWatcher.h"
//#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Misc/ScopedSlowTask.h"
//#include "WwiseEventDragDropOp.h"
//#include "Widgets/Input/SHyperlink.h"

#include "WaapiPlayerAssetManager.h"

//#include "AkWaapiClient.h"
//#include "AkSettings.h"


#define LOCTEXT_NAMESPACE "WaapiPlayer"


void SSwitchOrStateListViewWidget::Construct(const FArguments& InArgs)
{
		ChildSlot
		[
			SNew(SScrollBox)
			.Orientation(EOrientation::Orient_Vertical)
			.ScrollBarAlwaysVisible(true)

			+ SScrollBox::Slot()
			[
				SAssignNew(ListView, SListView<WaapiSwitchOrStateDataPtr>)
				.ListItemsSource(&DataListPtr)
				.OnGenerateRow(this, &SSwitchOrStateListViewWidget::OnGenerateRow)
				.OnSelectionChanged(this, &SSwitchOrStateListViewWidget::OnSelectionChanged)
				.ScrollbarVisibility(EVisibility::All)
			]
		];
}

SSwitchOrStateListViewWidget::SSwitchOrStateListViewWidget()
{
}

SSwitchOrStateListViewWidget::~SSwitchOrStateListViewWidget()
{
}

TSharedRef<ITableRow> SSwitchOrStateListViewWidget::OnGenerateRow(WaapiSwitchOrStateDataPtr DataPtr, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<WaapiSwitchOrStateDataPtr>>, OwnerTable)
			[
				SNew(SPlayerComboWidget)
				.GroupName(DataPtr->SwitchOrStateGroup)
				.OptionList(DataPtr->SwitchOrState)
				.ShowWidget(bCanShowWidget)
			];
}


void SSwitchOrStateListViewWidget::OnSelectionChanged(WaapiSwitchOrStateDataPtr DataPtr, ESelectInfo::Type type)
{

}


void SSwitchOrStateListViewWidget::SetItemList(TArray<WaapiSwitchOrStateDataPtr> List)
{
	if (List.Num() <= 0)
		return;

	DataListPtr = List;
	ListView->RequestListRefresh();
}

//==============================================================================================================
//SRtpcListViewWidget::SRtpcListViewWidget()
//{
//}
//
//SRtpcListViewWidget::~SRtpcListViewWidget()
//{
//}
//
//void SRtpcListViewWidget::Construct(const FArguments & InArgs)
//{
//}
//
//TSharedRef<ITableRow> SRtpcListViewWidget::OnGenerateRow(WaapiRtpcDataPtr DataPtr, const TSharedRef<STableViewBase>& OwnerTable)
//{
//	return TSharedRef<ITableRow>();
//}
//
//void SRtpcListViewWidget::OnSelectionChanged(WaapiRtpcDataPtr DataPtr, ESelectInfo::Type type)
//{
//}
//
//void SRtpcListViewWidget::SetItemList(TArray<WaapiRtpcDataPtr> List)
//{
//}



//==============================================================================================================
SPlayerComboWidget::SPlayerComboWidget()
{
	
}


SPlayerComboWidget::~SPlayerComboWidget()
{

}

void SPlayerComboWidget::Construct(const FArguments& InArgs)
{   
	CollectionsComboList = InArgs._OptionList;

	ChildSlot
	[
		SNew(SBorder)
		.Padding(4)
		.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
		[
			SNew(SOverlay)

			// Picker
			+ SOverlay::Slot()
			.VAlign(VAlign_Fill)
			[
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
						.Text(LOCTEXT("PlayerSwitchGroupTitle", "Switch Group"))
					]

					+SHorizontalBox::Slot()
					.HAlign(EHorizontalAlignment::HAlign_Fill)
					[
						SNew(STextBlock)
						.Text(FText::FromString(InArgs._GroupName))
					]
				]

				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)

					+SHorizontalBox::Slot()
					.HAlign(EHorizontalAlignment::HAlign_Fill)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("PlayerSwitchesTitle", "Switches"))
					]

					+SHorizontalBox::Slot()
					.HAlign(EHorizontalAlignment::HAlign_Fill)
					[
						SNew(SComboBox<TSharedPtr<FName>>)
						.OptionsSource(&CollectionsComboList)
						.OnGenerateWidget(this, &SPlayerComboWidget::GenerateCollectionFilterItem)
						//.OnSelectionChanged(this, &SReferenceViewer::HandleCollectionFilterChanged)
						//.ToolTipText(this, &SReferenceViewer::GetCollectionFilterText)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("PlayerSwitchesName", "Switches"))
						]
					]

				]
			]
		]
	];

}

TSharedRef<SWidget> SPlayerComboWidget::GenerateCollectionFilterItem(TSharedPtr<FName> InItem)
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




#undef LOCTEXT_NAMESPACE

