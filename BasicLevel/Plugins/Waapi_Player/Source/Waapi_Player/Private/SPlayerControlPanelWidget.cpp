// Copyright (c) 2006-2012 Audiokinetic Inc. / All Rights Reserved

/*------------------------------------------------------------------------------------
	SWwisePicker.cpp
------------------------------------------------------------------------------------*/

#include "SPlayerControlPanelWidget.h"
#include "AkAudioDevice.h"
//#include "AkAudioBankGenerationHelpers.h"
#include "AkAudioStyle.h"
#include "WaapiPlayerStyle.h"
//#include "WwisePicker/SWwisePicker.h"
//#include "WwisePicker/WwiseWwuParser.h"
//#include "DirectoryWatcherModule.h"
//#include "IDirectoryWatcher.h"
//#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Input/SComboBox.h"
#include "Misc/ScopedSlowTask.h"
//#include "WwiseEventDragDropOp.h"
//#include "Widgets/Input/SHyperlink.h"

#include "WaapiPlayerAssetManager.h"
#include "Widgets/Layout/SScrollBox.h"
#include "SPlayerListViewWidget.h"
#include "WaapiPlayerSqlManager.h"

//#include "AkWaapiClient.h"
//#include "AkSettings.h"


#define LOCTEXT_NAMESPACE "WaapiPlayer"



SPlayerControlPanelWidget::SPlayerControlPanelWidget()
{

}




SPlayerControlPanelWidget::~SPlayerControlPanelWidget()
{

}

void SPlayerControlPanelWidget::Construct(const FArguments& InArgs)
{   
	SwitchDataList.Empty();
	RtpcDataList.Empty();
	Targets = InArgs._TargetObjects;

	for (auto Target : Targets)
	{
		SwitchDataList.Append(Target->TargetExtraData.SwitchOrStateObjects);
		RtpcDataList.Append(Target->TargetExtraData.RtpcObjects);
	}

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

				// Search
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 1, 0, 3)
				[
					//SNew(SHorizontalBox)

					//+ SHorizontalBox::Slot()
					//.AutoWidth()
					//[
					//	InArgs._SearchContent.Widget
					//]

					//+ SHorizontalBox::Slot()
					//.FillWidth(1.0f)
					//[
					//	SNew(SSearchBox)
					//	.HintText( LOCTEXT( "WwisePickerSearchTooltip", "Search Wwise Item" ) )
					//	.OnTextChanged( this, &SPlayerControlPanelWidget::OnSearchBoxChanged )
					//	.SelectAllTextWhenFocused(false)
					//	.DelayChangeNotificationsWhileTyping(true)
					//]

					SNew(STextBlock)
					.Visibility(EVisibility::Visible)
					.AutoWrapText(true)
					.Justification(ETextJustify::Center)
					.Text(LOCTEXT("Empty", "GO GO GO!!!"))
				]

				// Tree title
				//+SVerticalBox::Slot()
				//.AutoHeight()
				//[
					//SNew(SHorizontalBox)
					//+ SHorizontalBox::Slot()
					//.AutoWidth()
					//.Padding(3.0f)
					//[
					//	SNew(SImage) 
					//	.Image(FAkAudioStyle::GetBrush(EWwiseTreeItemType::Project))
					//]

					//+ SHorizontalBox::Slot()
					//.AutoWidth()
					//.Padding(0,0,3,0)
					//[
					//	SNew(STextBlock)
					//	.Font( FEditorStyle::GetFontStyle("ContentBrowser.SourceTitleFont") )
					//	.Text( this, &SPlayerControlPanelWidget::GetProjectName )
					//	.Visibility(InArgs._ShowTreeTitle ? EVisibility::Visible : EVisibility::Collapsed)
					//]

					//+ SHorizontalBox::Slot()
					//.FillWidth(1)
					//[
					//	SNew( SSpacer )
					//]

					//+ SHorizontalBox::Slot()
					//.AutoWidth()
					//[
					//	SNew(SButton)
					//	.Text(LOCTEXT("AkPickerPopulate", "Populate"))
					//	.OnClicked(this, &SPlayerControlPanelWidget::OnPopulateClicked)
					//]
				//]

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
					SAssignNew(SwitchListView, SSwitchOrStateListViewWidget)
				]

				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SAssignNew(RtpcListView, SRtpcListViewWidget)
				]
			]
		]
	];

	SwitchListView->SetItemList(SwitchDataList);
	RtpcListView->SetItemList(RtpcDataList);
}

//TSharedRef<SWidget> SPlayerControlPanelWidget::GenerateCollectionFilterItem(TSharedPtr<FName> InItem)
//{
//	FText ItemAsText = FText::FromName(*InItem);
//	return
//		SNew(SBox)
//		.WidthOverride(300)
//		[
//			SNew(STextBlock)
//			.Text(ItemAsText)
//			.ToolTipText(ItemAsText)
//		];
//}




#undef LOCTEXT_NAMESPACE