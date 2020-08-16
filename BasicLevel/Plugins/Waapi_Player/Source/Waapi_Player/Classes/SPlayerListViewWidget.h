#pragma once

//#include "WaapiPicker/WwiseTreeItem.h"
//#include "WwiseWwuParser.h"
//#include "Misc/TextFilter.h"
//#include "Widgets/Views/STreeView.h"
#include "Widgets/SCompoundWidget.h"
#include "WaapiPlayerSqlManager.h"
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
	TArray<WaapiSwitchOrStateDataPtr> DataListPtr;
	TSharedPtr<SwitchOrStateListViewType> ListView;

	bool bCanShowWidget;
};


//class SRtpcListViewWidget : public SCompoundWidget
//{
//public:
//	SLATE_BEGIN_ARGS(SRtpcListViewWidget) {}
//	SLATE_END_ARGS();
//
//	SRtpcListViewWidget();
//	~SRtpcListViewWidget();
//
//	void Construct(const FArguments& InArgs);
//
//
//	TSharedRef<ITableRow> OnGenerateRow(WaapiRtpcDataPtr DataPtr, const TSharedRef<STableViewBase>& OwnerTable);
//
//	void OnSelectionChanged(WaapiRtpcDataPtr DataPtr, ESelectInfo::Type type);
//
//	void SetItemList(TArray<WaapiRtpcDataPtr> List);
//
//private:
//	TArray<WaapiRtpcDataPtr> DataListPtr;
//	TSharedPtr<RtpcListViewType> ListView;
//};


class SPlayerComboWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPlayerComboWidget)
		: _GroupName("")
		, _OptionList()
		, _ShowWidget(true)
		{}
		//, _ShowSeparator(true)
		//, _AllowContextMenu(true)
		//, _SelectionMode( ESelectionMode::Single)

	//	/** Content displayed to the left of the search bar */
	//SLATE_NAMED_SLOT(FArguments, SearchContent)

	//	/** If true, the search box will be focus the frame after construction */
		SLATE_ARGUMENT(FString, GroupName)

	//	/** If true, The tree title will be displayed */
		SLATE_ARGUMENT(TArray<TSharedPtr<FName>>, OptionList)

	//	/** If true, The tree search bar will be displayed */
		SLATE_ARGUMENT(bool, ShowWidget)

	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);
	
	SPlayerComboWidget();
	
	~SPlayerComboWidget();

	//static const FName PlayerMainWidgetTabName;

    
    //virtual void Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime ) override;


	TSharedRef<SWidget> GenerateCollectionFilterItem(TSharedPtr<FName> InItem);

private:
	/** List of collection filter options */
	TArray<TSharedPtr<FName>> CollectionsComboList;

};