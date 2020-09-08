#pragma once

#include "Widgets/SCompoundWidget.h"



class SPlayerSwitchWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPlayerSwitchWidget)
		: _GroupName("")
		, _OptionList()
		, _IsSwitch(true)
		, _ShowWidget(true)
		{}

	//	/** If true, the search box will be focus the frame after construction */
		SLATE_ARGUMENT(FString, GroupName)

		SLATE_ARGUMENT(bool, IsSwitch)

	//	/** If true, The tree title will be displayed */
		SLATE_ARGUMENT(TArray<TSharedPtr<FName>>, OptionList)

	//	/** If true, The tree search bar will be displayed */
		SLATE_ARGUMENT(bool, ShowWidget)

	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);
	
	SPlayerSwitchWidget();
	
	~SPlayerSwitchWidget();

	TSharedRef<SWidget> OnGenerateWidget(TSharedPtr<FName> InItem);
	void OnSelectionChanged(TSharedPtr<FName> SelectedItems, ESelectInfo::Type Type);

private:
	/** List of collection filter options */
	TArray<TSharedPtr<FName>> CollectionsComboList;
	bool bIsSwitch;

	TSharedPtr<STextBlock> SwitchOrStateTitleText;
	TSharedPtr<STextBlock> SelectedSwitchOptionText;

};

class SPlayerRtpcWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPlayerRtpcWidget)
		: _RtpcName("")
		, _DefaultValue(0.0f)
		, _MinValue(0.0f)
		, _MaxValue(100.0f)
		, _ShowWidget(true)
	{}

		SLATE_ARGUMENT(FString, RtpcName)

		SLATE_ARGUMENT(float, DefaultValue)

		SLATE_ARGUMENT(float, MinValue)

		SLATE_ARGUMENT(float, MaxValue)

		SLATE_ARGUMENT(bool, ShowWidget)

	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

	SPlayerRtpcWidget();

	~SPlayerRtpcWidget();

	void OnValueChanged(float Value);

};