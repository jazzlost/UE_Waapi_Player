// Copyright (c) 2006-2012 Audiokinetic Inc. / All Rights Reserved

/*------------------------------------------------------------------------------------
	SWwisePicker.cpp
------------------------------------------------------------------------------------*/

#include "SPlayerListViewWidget.h"
#include "WaapiPlayerStyle.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SComboBox.h"
#include "Misc/ScopedSlowTask.h"

#include "WaapiPlayerAssetManager.h"
#include "SPlayerComboWidget.h"


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
				.ListItemsSource(&SwitchDataListPtr)
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
				SNew(SPlayerSwitchWidget)
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

	SwitchDataListPtr = List;
	ListView->RequestListRefresh();
}

//==============================================================================================================
SRtpcListViewWidget::SRtpcListViewWidget()
{
}

SRtpcListViewWidget::~SRtpcListViewWidget()
{
}

void SRtpcListViewWidget::Construct(const FArguments & InArgs)
{
	ChildSlot
		[
			SNew(SScrollBox)
			.Orientation(EOrientation::Orient_Vertical)
			.ScrollBarAlwaysVisible(true)

			+ SScrollBox::Slot()
			[
				SAssignNew(ListView, SListView<WaapiRtpcDataPtr>)
				.ListItemsSource(&RtpcDataListPtr)
				.OnGenerateRow(this, &SRtpcListViewWidget::OnGenerateRow)
				.OnSelectionChanged(this, &SRtpcListViewWidget::OnSelectionChanged)
				.ScrollbarVisibility(EVisibility::All)
			]
		];
}

TSharedRef<ITableRow> SRtpcListViewWidget::OnGenerateRow(WaapiRtpcDataPtr DataPtr, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<WaapiRtpcDataPtr>>, OwnerTable)
		[
			SNew(SPlayerRtpcWidget)
			.RtpcName(DataPtr->RtpcName)
			.DefaultValue(DataPtr->DefaultValue)
			.MinValue(DataPtr->MinValue)
			.MaxValue(DataPtr->MaxValue)
		];
}

void SRtpcListViewWidget::OnSelectionChanged(WaapiRtpcDataPtr DataPtr, ESelectInfo::Type type)
{
}

void SRtpcListViewWidget::SetItemList(TArray<WaapiRtpcDataPtr> List)
{
	if (List.Num() <= 0)
		return;

	RtpcDataListPtr = List;
	ListView->RequestListRefresh();
}




#undef LOCTEXT_NAMESPACE

