#pragma once

#include "Widgets/SCompoundWidget.h"
#include "WaapiTargetObject.h"
#include "Widgets/Views/SListView.h"

typedef TSharedPtr<WaapiSwitchStateObject> WaapiSwitchOrStateDataPtr;
typedef TSharedPtr<WaapiRtpcObject> WaapiRtpcDataPtr;
typedef SListView<WaapiSwitchOrStateDataPtr> SwitchOrStateListViewType;
typedef SListView<WaapiRtpcDataPtr> RtpcListViewType;


class SSwitchOrStateListViewWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSwitchOrStateListViewWidget) {}
	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

	SSwitchOrStateListViewWidget();
	~SSwitchOrStateListViewWidget();

	TSharedRef<ITableRow> OnGenerateRow(WaapiSwitchOrStateDataPtr DataPtr, const TSharedRef<STableViewBase>& OwnerTable);

	void OnSelectionChanged(WaapiSwitchOrStateDataPtr DataPtr, ESelectInfo::Type type);

	void SetItemList(TArray<WaapiSwitchOrStateDataPtr> List);

private:
	TArray<WaapiSwitchOrStateDataPtr> SwitchDataListPtr;
	TSharedPtr<SwitchOrStateListViewType> ListView;

	bool bCanShowWidget;
};


class SRtpcListViewWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRtpcListViewWidget) {}
	SLATE_END_ARGS();

	SRtpcListViewWidget();
	~SRtpcListViewWidget();

	void Construct(const FArguments& InArgs);


	TSharedRef<ITableRow> OnGenerateRow(WaapiRtpcDataPtr DataPtr, const TSharedRef<STableViewBase>& OwnerTable);

	void OnSelectionChanged(WaapiRtpcDataPtr DataPtr, ESelectInfo::Type type);

	void SetItemList(TArray<WaapiRtpcDataPtr> List);

private:
	TArray<WaapiRtpcDataPtr> RtpcDataListPtr;
	TSharedPtr<RtpcListViewType> ListView;

	bool bCanShowWidget;

};
