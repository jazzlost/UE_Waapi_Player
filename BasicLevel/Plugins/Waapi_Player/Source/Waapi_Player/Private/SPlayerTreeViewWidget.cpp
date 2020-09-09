// Copyright (c) 2006-2012 Audiokinetic Inc. / All Rights Reserved

/*------------------------------------------------------------------------------------
	SWwisePicker.cpp
------------------------------------------------------------------------------------*/

#include "SPlayerTreeViewWidget.h"
#include "AkAudioDevice.h"
//#include "AkAudioBankGenerationHelpers.h"
#include "AkAudioStyle.h"
#include "WaapiPlayerStyle.h"
//#include "WwisePicker/SWwisePicker.h"
//#include "WwisePicker/WwiseWwuParser.h"
//#include "DirectoryWatcherModule.h"
//#include "IDirectoryWatcher.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SSpacer.h"
#include "Misc/ScopedSlowTask.h"
//#include "WwiseEventDragDropOp.h"
#include "Widgets/Input/SHyperlink.h"

#include "WaapiPlayer.h"
#include "WaapiPlayerAssetManager.h"
#include "WaapiPlayerAssetEditor.h"
#include "Modules/ModuleManager.h"

//#include "AkWaapiClient.h"
//#include "AkSettings.h"


#define LOCTEXT_NAMESPACE "WaapiPlayer"

//const FName SPlayerTreeViewWidget::WwisePickerTabName = FName("WwisePicker");


SPlayerTreeViewWidget::SPlayerTreeViewWidget()
{
	AllowTreeViewDelegates = true;
	//isPickerVisible = !FAkWaapiClient::IsProjectLoaded();
}

//void SPlayerTreeViewWidget::RemoveClientCallbacks()
//{
//	auto pWaapiClient = FAkWaapiClient::Get();
//	if (pWaapiClient != nullptr)
//	{
//		if (ProjectLoadedHandle.IsValid())
//		{
//			pWaapiClient->OnProjectLoaded.Remove(ProjectLoadedHandle);
//			ProjectLoadedHandle.Reset();
//		}
//		if (ConnectionLostHandle.IsValid())
//		{
//			pWaapiClient->OnConnectionLost.Remove(ConnectionLostHandle);
//			ConnectionLostHandle.Reset();
//		}
//	}
//}

//void SPlayerTreeViewWidget::UpdateDirectoryWatcher()
//{
//	FString OldProjectFolder = ProjectFolder;
//	ProjectFolder = FPaths::GetPath(WwiseBnkGenHelper::GetLinkedProjectPath());
//	ProjectName = FPaths::GetCleanFilename(WwiseBnkGenHelper::GetLinkedProjectPath());
//
//	static const FName DirectoryWatcherModuleName = TEXT("DirectoryWatcher");
//	FDirectoryWatcherModule& DirectoryWatcherModule = FModuleManager::LoadModuleChecked<FDirectoryWatcherModule>(DirectoryWatcherModuleName);
//	if (ProjectDirectoryModifiedDelegateHandle.IsValid())
//	{
//		DirectoryWatcherModule.Get()->UnregisterDirectoryChangedCallback_Handle(OldProjectFolder, ProjectDirectoryModifiedDelegateHandle);
//	}
//
//	DirectoryWatcherModule.Get()->RegisterDirectoryChangedCallback_Handle(
//		ProjectFolder
//		, IDirectoryWatcher::FDirectoryChanged::CreateRaw(this, &SPlayerTreeViewWidget::OnProjectDirectoryChanged)
//		, ProjectDirectoryModifiedDelegateHandle
//		);
//
//}

//void SPlayerTreeViewWidget::OnProjectDirectoryChanged(const TArray<struct FFileChangeData>& ChangedFiles)
//{
//	bool bFoundWorkUnit = false;
//	for(auto File : ChangedFiles)
//	{
//		FPaths::NormalizeDirectoryName(File.Filename);
//		if(File.Filename.EndsWith(FString(TEXT(".wwu"))) &&
//			(File.Filename.Contains(TEXT("/Events/")) || File.Filename.Contains(TEXT("/Master-Mixer Hierarchy/")) || File.Filename.Contains(TEXT("/Virtual Acoustics/"))) )
//		{
//			bFoundWorkUnit = true;
//		}
//	}
//
//	if(bFoundWorkUnit)
//	{
//		OnPopulateClicked();
//	}
//}

SPlayerTreeViewWidget::~SPlayerTreeViewWidget()
{
	RootItems.Empty();
	//RemoveClientCallbacks();
	//auto pWaapiClient = FAkWaapiClient::Get();
	//if (pWaapiClient != nullptr)
	//{
	//	pWaapiClient->OnClientBeginDestroy.Remove(ClientBeginDestroyHandle);
	//}

	//static const FName DirectoryWatcherModuleName = TEXT("DirectoryWatcher");
	//FDirectoryWatcherModule& DirectoryWatcherModule = FModuleManager::LoadModuleChecked<FDirectoryWatcherModule>(DirectoryWatcherModuleName);
	//DirectoryWatcherModule.Get()->UnregisterDirectoryChangedCallback_Handle(ProjectFolder, ProjectDirectoryModifiedDelegateHandle);
}

void SPlayerTreeViewWidget::Construct(const FArguments& InArgs)
{
	//UpdateDirectoryWatcher();
	SearchBoxFilter = MakeShareable( new StringFilter( StringFilter::FItemToStringArray::CreateSP( this, &SPlayerTreeViewWidget::PopulateSearchStrings ) ) );
	SearchBoxFilter->OnChanged().AddSP( this, &SPlayerTreeViewWidget::FilterUpdated );

    //UAkSettings* AkSettings = GetMutableDefault<UAkSettings>();
    //AkSettings->bRequestRefresh = false;
    
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
				.Visibility(this, &SPlayerTreeViewWidget::isPickerAllowed)

				// Search
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 1, 0, 3)
				[
					SNew(SHorizontalBox)

					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						InArgs._SearchContent.Widget
					]

					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						SNew(SSearchBox)
						.HintText( LOCTEXT( "WwisePickerSearchTooltip", "Search Wwise Item" ) )
						.OnTextChanged( this, &SPlayerTreeViewWidget::OnSearchBoxChanged )
						.SelectAllTextWhenFocused(false)
						.DelayChangeNotificationsWhileTyping(true)
					]
				]

				// Tree title
				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(3.0f)
					[
						SNew(SImage) 
						.Image(FAkAudioStyle::GetBrush(EWwiseTreeItemType::Project))
					]

					//+ SHorizontalBox::Slot()
					//.AutoWidth()
					//.Padding(0,0,3,0)
					//[
					//	SNew(STextBlock)
					//	.Font( FEditorStyle::GetFontStyle("ContentBrowser.SourceTitleFont") )
					//	.Text( this, &SPlayerTreeViewWidget::GetProjectName )
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
					//	.OnClicked(this, &SPlayerTreeViewWidget::OnPopulateClicked)
					//]
				]

				// Separator
				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 0, 0, 1)
				[
					SNew(SSeparator)
					.Visibility( ( InArgs._ShowSeparator) ? EVisibility::Visible : EVisibility::Collapsed )
				]
				
				// Tree
				+SVerticalBox::Slot()
				.FillHeight(1.f)
				[
					SAssignNew(TreeViewPtr, STreeView< TSharedPtr<FWaapiPlayerTreeItem> >)
					.TreeItemsSource(&RootItems)
					.OnGenerateRow( this, &SPlayerTreeViewWidget::GenerateRow )
					//.OnItemScrolledIntoView( this, &SPathView::TreeItemScrolledIntoView )
					.ItemHeight(18)
					.SelectionMode(InArgs._SelectionMode)
					.OnSelectionChanged(this, &SPlayerTreeViewWidget::TreeSelectionChanged)
					.OnExpansionChanged(this, &SPlayerTreeViewWidget::TreeExpansionChanged)
					.OnGetChildren( this, &SPlayerTreeViewWidget::GetChildrenForTree )
					//.OnSetExpansionRecursive( this, &SPathView::SetTreeItemExpansionRecursive )
					//.OnContextMenuOpening(this, &SPathView::MakePathViewContextMenu)
					.ClearSelectionOnClick(false)
					.OnMouseButtonDoubleClick(this, &SPlayerTreeViewWidget::OnTreeItemsDoubleClicked)
				]
			]

			// Empty Picker
			//+ SOverlay::Slot()
			//.VAlign(VAlign_Center)
			//.HAlign(HAlign_Center)
			//[
			//	SNew(SVerticalBox)
			//	+ SVerticalBox::Slot()
			//	.VAlign(VAlign_Center)
			//	.HAlign(HAlign_Center)
			//	.AutoHeight()
			//	[
			//		SNew(STextBlock)
			//		.Visibility(this, &SPlayerTreeViewWidget::isWarningVisible)
			//		.AutoWrapText(true)
			//		.Justification(ETextJustify::Center)
			//		.Text(LOCTEXT("EmptyWwiseTree", "WAAPI connection available; the Wwise Picker has been disabled. Please use the WAAPI Picker."))
			//	]
				/*+ SVerticalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SHyperlink)
					.Visibility(this, &SPlayerTreeViewWidget::isWarningVisible)
					.Text(LOCTEXT("WaapiDucumentation", "For more informaton, please Visit Waapi Documentation."))
					.ToolTipText(LOCTEXT("WaapiDucumentationTooltip", "Opens Waapi documentation in a new browser window"))
					.OnNavigate_Lambda([=]() { FPlatformProcess::LaunchURL(*FString("https://www.audiokinetic.com/library/?source=SDK&id=waapi.html"), nullptr, nullptr); })
				]*/
			//]
		]
	];

	//auto pWaapiClient = FAkWaapiClient::Get();
	//if (pWaapiClient)
	//{
		/* Empty the tree when we have the same project */
		//ProjectLoadedHandle = pWaapiClient->OnProjectLoaded.AddLambda([this]()
		//{
		//	isPickerVisible = false;
		//	RootItems.Empty();
		//	ConstructTree();
		//});
		/* Construct the tree when we have different projects */
		//ConnectionLostHandle = pWaapiClient->OnConnectionLost.AddLambda([this]()
		//{
		//	isPickerVisible = true;
		//	ConstructTree();
		//});
		//ClientBeginDestroyHandle = pWaapiClient->OnClientBeginDestroy.AddLambda([this]()
		//{
		//	RemoveClientCallbacks();
		//});
	//}

	//OnPopulateClicked();
	//WaapiPlayerAssetManager& AssetManager = WaapiPlayerAssetManager::Get();
	//AssetManager.Init();

	ForceRefresh();
	TreeViewPtr->RequestTreeRefresh();
	ExpandFirstLevel();
}

EVisibility SPlayerTreeViewWidget::isPickerAllowed() const
{
	if (isPickerVisible)
		return EVisibility::Visible;
	return EVisibility::Hidden;
}

EVisibility SPlayerTreeViewWidget::isWarningVisible() const
{
	if (!isPickerVisible)
		return EVisibility::Visible;
	return EVisibility::Hidden;
}

//void SPlayerTreeViewWidget::Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime )
//{
//	UAkSettings* AkSettings = GetMutableDefault<UAkSettings>();
//    if(AkSettings->bRequestRefresh)
//    {
//        ForceRefresh();
//        AkSettings->bRequestRefresh = false;
//    }
//}

void SPlayerTreeViewWidget::ForceRefresh()
{
	//FWwiseWwuParser::ForcePopulate();
	//UpdateDirectoryWatcher();
	WaapiPlayerAssetManager& AssetManager = WaapiPlayerAssetManager::Get();
	AssetManager.Init();

	ConstructTree();
}

//FText SPlayerTreeViewWidget::GetProjectName() const
//{
//	return FText::FromString(ProjectName);
//}

//FReply SPlayerTreeViewWidget::OnPopulateClicked()
//{
//	//FWwiseWwuParser::Populate();
//	ConstructTree();
//	//return FReply::Handled();
//}

void SPlayerTreeViewWidget::ConstructTree()
{
	//if (!FAkWaapiClient::IsProjectLoaded())
	//{
		//RootItems.Empty(EWwiseTreeItemType::NUM_DRAGGABLE_WWISE_ITEMS);
		//EWwiseTreeItemType::Type CurrentType = EWwiseTreeItemType::Event;

		//while ((int)CurrentType < (int)EWwiseTreeItemType::NUM_DRAGGABLE_WWISE_ITEMS)
		//{
			//TSharedPtr<FWaapiPlayerTreeItem> NewRoot = FWwiseWwuParser::GetTree(SearchBoxFilter, RootItems.Num() > CurrentType ? RootItems[CurrentType] : nullptr, CurrentType);
			//TSharedPtr<FWaapiPlayerTreeItem> NewRoot = MakeShareable(new FWaapiPlayerTreeItem(TEXT("WaapiRoot"), TEXT("Path/Root"), nullptr, EWaapiPlayerTreeItemType::Folder));
			//NewRoot->Children.Add(MakeShareable(new FWaapiPlayerTreeItem(TEXT("WaapiChild"), TEXT("Path/Root/Children"), NewRoot, EWaapiPlayerTreeItemType::Event)));
			//CurrentType = (EWwiseTreeItemType::Type)(((int)CurrentType) + 1);

			WaapiPlayerAssetManager& AssetManager = WaapiPlayerAssetManager::Get();
			RootItems.Add(AssetManager.RootItem);
		//}		
	//}
	RestoreTreeExpansion(RootItems);
	TreeViewPtr->RequestTreeRefresh();
}

void SPlayerTreeViewWidget::ExpandFirstLevel()
{
	// Expand root items and first-level work units.
	for(int32 i = 0; i < RootItems.Num(); i++)
	{
		TreeViewPtr->SetItemExpansion(RootItems[i], true);
	}
}

void SPlayerTreeViewWidget::ExpandParents(TSharedPtr<FWaapiPlayerTreeItem> Item)
{
	if(Item->Parent.IsValid())
	{
		ExpandParents(Item->Parent.Pin());
		TreeViewPtr->SetItemExpansion(Item->Parent.Pin(), true);
	}
}

TSharedRef<ITableRow> SPlayerTreeViewWidget::GenerateRow( TSharedPtr<FWaapiPlayerTreeItem> TreeItem, const TSharedRef<STableViewBase>& OwnerTable )
{
	check(TreeItem.IsValid());

	EVisibility RowVisibility = TreeItem->IsVisible ? EVisibility::Visible : EVisibility::Collapsed;
	//EVisibility RowVisibility = EVisibility::Visible;

	TSharedPtr<ITableRow> NewRow = SNew( STableRow< TSharedPtr<FWaapiPlayerTreeItem> >, OwnerTable )
		.OnDragDetected( this, &SPlayerTreeViewWidget::OnDragDetected )
		.Visibility(RowVisibility)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(0, 1, 2, 1)
			.VAlign(VAlign_Center)
			[
				SNew(SImage) 
				//.Image(FAkAudioStyle::GetBrush(EWwiseTreeItemType::Event))
				.Image(FWaapiPlayerStyle::GetBrush(TreeItem->ItemType))
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TreeItem->DisplayName))
				.HighlightText(this, &SPlayerTreeViewWidget::GetHighlightText)
			]
		];

	TreeItem->TreeRow = NewRow.Get();

	return NewRow.ToSharedRef();
}

void SPlayerTreeViewWidget::GetChildrenForTree( TSharedPtr< FWaapiPlayerTreeItem > TreeItem, TArray< TSharedPtr<FWaapiPlayerTreeItem> >& OutChildren )
{
	OutChildren = TreeItem->Children;
}

//FReply SPlayerTreeViewWidget::OnDragDetected(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
//{
//	if ( MouseEvent.IsMouseButtonDown( EKeys::LeftMouseButton ) )
//	{
//		TArray<TSharedPtr<FWaapiPlayerTreeItem>> SelectedItems = TreeViewPtr->GetSelectedItems();
//		return FReply::Handled().BeginDragDrop(FWwiseEventDragDropOp::New(SelectedItems));
//	}
//
//	return FReply::Unhandled();
//}

void SPlayerTreeViewWidget::PopulateSearchStrings( const FString& FolderName, OUT TArray< FString >& OutSearchStrings ) const
{
	OutSearchStrings.Add( FolderName );
}

void SPlayerTreeViewWidget::OnSearchBoxChanged( const FText& InSearchText )
{
	SearchBoxFilter->SetRawFilterText( InSearchText );
}

FText SPlayerTreeViewWidget::GetHighlightText() const
{
	return SearchBoxFilter->GetRawFilterText();
}

void SPlayerTreeViewWidget::FilterUpdated()
{
	FScopedSlowTask SlowTask(2.f, LOCTEXT("AK_PopulatingPicker", "Populating Wwise Picker..."));
	SlowTask.MakeDialog();
	for(int32 i = 0; i < RootItems.Num(); i++)
	{
		ApplyFilter(RootItems[i]);
	}
	TreeViewPtr->RequestTreeRefresh();
}

void SPlayerTreeViewWidget::SetItemVisibility(TSharedPtr<FWaapiPlayerTreeItem> Item, bool IsVisible)
{
	if( Item.IsValid() )
	{
		if( IsVisible )
		{
			// Propagate visibility to parents.
			SetItemVisibility(Item->Parent.Pin(), IsVisible);
		}
		Item->IsVisible = IsVisible;
		if( Item->TreeRow != NULL )
		{
			TSharedRef<SWidget> wid = Item->TreeRow->AsWidget();
			Item->TreeRow->AsWidget()->SetVisibility(IsVisible ? EVisibility::Visible : EVisibility::Collapsed);
		}
	}
}

void SPlayerTreeViewWidget::ApplyFilter(TSharedPtr<FWaapiPlayerTreeItem> ItemToFilter)
{

	EWwiseTreeItemType::Type CurrentType = EWwiseTreeItemType::Event;
	//while ((int)CurrentType < (int)EWwiseTreeItemType::NUM_DRAGGABLE_WWISE_ITEMS)
	//{
	//	TSharedPtr<FWaapiPlayerTreeItem> NewRoot = FWwiseWwuParser::GetTree(SearchBoxFilter, RootItems[CurrentType], CurrentType);
	//	RootItems[CurrentType] = NewRoot;
	//	CurrentType = (EWwiseTreeItemType::Type)(((int)CurrentType) + 1);
	//}

	AllowTreeViewDelegates = false;
	RestoreTreeExpansion(RootItems);
	AllowTreeViewDelegates = true;
}

void SPlayerTreeViewWidget::RestoreTreeExpansion(const TArray< TSharedPtr<FWaapiPlayerTreeItem> >& Items)
{
	for(int i = 0; i < Items.Num(); i++)
	{
		if( LastExpandedPaths.Contains(Items[i]->FolderPath) )
		{
			TreeViewPtr->SetItemExpansion(Items[i], true);
		}
		RestoreTreeExpansion(Items[i]->Children);
	}
}

void SPlayerTreeViewWidget::TreeSelectionChanged( TSharedPtr< FWaapiPlayerTreeItem > TreeItem, ESelectInfo::Type /*SelectInfo*/ )
{
	if( AllowTreeViewDelegates )
	{
		const TArray<TSharedPtr<FWaapiPlayerTreeItem>> SelectedItems = TreeViewPtr->GetSelectedItems();

		LastSelectedPaths.Empty();
		for (int32 ItemIdx = 0; ItemIdx < SelectedItems.Num(); ++ItemIdx)
		{
			const TSharedPtr<FWaapiPlayerTreeItem> Item = SelectedItems[ItemIdx];
			if ( Item.IsValid() )
			{
				LastSelectedPaths.Add(Item->FolderPath);
			}
		}
	}
}

void SPlayerTreeViewWidget::TreeExpansionChanged( TSharedPtr< FWaapiPlayerTreeItem > TreeItem, bool bIsExpanded )
{
	if( AllowTreeViewDelegates )
	{
		TSet<TSharedPtr<FWaapiPlayerTreeItem>> ExpandedItemSet;
		TreeViewPtr->GetExpandedItems(ExpandedItemSet);

		LastExpandedPaths.Empty();
		for (auto ExpandedItemIt = ExpandedItemSet.CreateConstIterator(); ExpandedItemIt; ++ExpandedItemIt)
		{
			const TSharedPtr<FWaapiPlayerTreeItem> Item = *ExpandedItemIt;
			if ( Item.IsValid() )
			{
				// Keep track of the last paths that we broadcasted for expansion reasons when filtering
				LastExpandedPaths.Add(Item->FolderPath);
			}
		}
	}
}

void SPlayerTreeViewWidget::OnTreeItemsDoubleClicked(TSharedPtr<FWaapiPlayerTreeItem> ClickedItem)
{
	FWaapiPlayerModule& WaapiPlayerModule =  FModuleManager::Get().GetModuleChecked<FWaapiPlayerModule>("WaapiPlayer");
	TSharedPtr<FWaapiPlayerAssetEditor> AssetEditor = WaapiPlayerModule.GetAssetEditor();

	if (AssetEditor.IsValid())
	{
		FString ClickedEventName = ClickedItem->DisplayName;
		AssetEditor->OnTreeItemSelected.Broadcast(ClickedEventName);
	}
}


#undef LOCTEXT_NAMESPACE