#pragma once

//#include "WaapiPicker/WwiseTreeItem.h"
//#include "WwiseWwuParser.h"
#include "Misc/TextFilter.h"
#include "Widgets/Views/STreeView.h"

#include "WaapiPlayerTreeItem.h"
#include "SPlayerListViewWidget.h"


class SPlayerControlPanelWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPlayerControlPanelWidget)
		: _TargetObjects()
		//: _FocusSearchBoxWhenOpened(false)
		//, _ShowTreeTitle(true)
		//, _ShowSearchBar(true)
		//, _ShowSeparator(true)
		//, _AllowContextMenu(true)
		//, _SelectionMode( ESelectionMode::Single)
		{}

	//	/** Content displayed to the left of the search bar */
	//SLATE_NAMED_SLOT(FArguments, SearchContent)

	//	/** If true, the search box will be focus the frame after construction */
	//	SLATE_ARGUMENT(bool, FocusSearchBoxWhenOpened)

	//	/** If true, The tree title will be displayed */
	//	SLATE_ARGUMENT(bool, ShowTreeTitle)

	//	/** If true, The tree search bar will be displayed */
	//	SLATE_ARGUMENT(bool, ShowSearchBar)

	//	/** If true, The tree search bar separator be displayed */
	//	SLATE_ARGUMENT(bool, ShowSeparator)

	//	/** If false, the context menu will be suppressed */
	//	SLATE_ARGUMENT(bool, AllowContextMenu)

	//	/** The selection mode for the tree view */
	//	SLATE_ARGUMENT(ESelectionMode::Type, SelectionMode)

		SLATE_ARGUMENT(TArray<UWaapiTargetObject*>, TargetObjects)

	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);
	
	SPlayerControlPanelWidget();
	
	~SPlayerControlPanelWidget();

	//static const FName PlayerMainWidgetTabName;

    
    //virtual void Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime ) override;


	//TSharedRef<SWidget> GenerateCollectionFilterItem(TSharedPtr<FName> InItem);

private:
	/** List of collection filter options */
	TSharedPtr<SSwitchOrStateListViewWidget> SwitchListView;
	TSharedPtr<SRtpcListViewWidget> RtpcListView;

	TArray<UWaapiTargetObject*> Targets;

	/**List Data*/
	TArray<WaapiSwitchOrStateDataPtr> SwitchDataList;
	TArray<WaapiRtpcDataPtr> RtpcDataList;
};