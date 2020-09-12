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
#include "WaapiPlayerPlayingObject.h"
#include "WaapiPlayer.h"

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
		//SwitchDataList.Append(Target->TargetExtraData.SwitchOrStateObjects);
		RtpcDataList.Append(Target->TargetExtraData.RtpcObjects);

		UWaapiPlayerPlayingObject* PreplayingObject = FWaapiPlayerModule::GetPreplayingObject();

		for (auto SwitchObject : Target->TargetExtraData.SwitchOrStateObjects)
		{
			if (SwitchObject->bSwitch)
			{
				SwitchDataList.Add(SwitchObject);
				PreplayingObject->SwitchPair.Add(SwitchObject->SwitchOrStateGroup, SwitchObject->SwitchOrState[0]->ToString());
			}
			else
			{
				StateDataList.Add(SwitchObject);
				PreplayingObject->StatePair.Add(SwitchObject->SwitchOrStateGroup, SwitchObject->SwitchOrState[0]->ToString());
			}
		}

		for (auto RtpcObject : Target->TargetExtraData.RtpcObjects)
		{
			PreplayingObject->RtpcPair.Add(RtpcObject->RtpcName, RtpcObject->DefaultValue);
		}
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
				//+ SVerticalBox::Slot()
				//.AutoHeight()
				//.Padding(0, 1, 0, 3)
				//[
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

					//SNew(STextBlock)
					//.Visibility(EVisibility::Visible)
					//.AutoWrapText(true)
					//.Justification(ETextJustify::Center)
					//.Text(LOCTEXT("Empty", "GO GO GO!!!"))
				//]

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
				.VAlign(EVerticalAlignment::VAlign_Fill)
				[
					SNew(SSeparator)
					.Visibility(EVisibility::Visible)
				]

				+SVerticalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Fill)
				.FillHeight(80)
				.Padding(0, 20, 0, 0)
				[
					SAssignNew(SwitchListView, SSwitchOrStateListViewWidget)
					.IsSwitch(true)
				]

				+ SVerticalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Fill)
				.FillHeight(80)
				.Padding(0, 20, 0, 0)
				[
					SAssignNew(StateListView, SSwitchOrStateListViewWidget)
					.IsSwitch(false)
				]

				+SVerticalBox::Slot()
				.FillHeight(80)
				.Padding(0, 20, 0, 0)
				[
					SAssignNew(RtpcListView, SRtpcListViewWidget)
				]
			]
		]
	];

	SwitchListView->SetItemList(SwitchDataList);
	StateListView->SetItemList(StateDataList);
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