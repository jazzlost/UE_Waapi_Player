// Copyright (c) 2006-2012 Audiokinetic Inc. / All Rights Reserved

/*------------------------------------------------------------------------------------
	WwiseTreeItem.h
------------------------------------------------------------------------------------*/
#pragma once
#include "Engine/GameEngine.h"
#include "Widgets/Views/STableRow.h"
/*------------------------------------------------------------------------------------
	WwiseTreeItem
------------------------------------------------------------------------------------*/

namespace EWaapiPlayerTreeItemType
{
	enum Type
	{
		Event,
		Folder
	};

	static const FString ItemNames[NUM_DRAGGABLE_WAAPI_ITEMS] = { TEXT("Event"), TEXT("AuxBus"), TEXT("AcousticTexture"), TEXT("ActorMixer") };
	static const FString DisplayNames[NUM_DRAGGABLE_WAAPI_ITEMS] = { TEXT("Events"), TEXT("Busses"), TEXT("VirtualAcoustics"), TEXT("ActorMixer") };
	static const FString FolderNames[NUM_DRAGGABLE_WAAPI_ITEMS] = { TEXT("Events"), TEXT("Master-Mixer Hierarchy"), TEXT("Virtual Acoustics"), TEXT("Actor-Mixer Hierarchy") };
	static const FString PickerLabel[NUM_DRAGGABLE_WAAPI_ITEMS] = { TEXT("Events"), TEXT("Auxiliary Busses"), TEXT("Textures"), TEXT("Actor Mixer") };

};

struct FWaapiPlayerTreeItem : public TSharedFromThis<FWaapiPlayerTreeItem>
{
	/** Name to display */
	FString DisplayName;
	/** The path of the tree item including the name */
	FString FolderPath;
	/** Type of the item */
	EWaapiPlayerTreeItemType::Type ItemType;
	/** Id of the item */
	//FGuid ItemId;

	/** The children of this tree item */
	TArray< TSharedPtr<FWaapiPlayerTreeItem> > Children;
	
	/** The number of children of this tree item requested from Wwise*/
	//uint32_t ChildCountInWwise;

	/** The parent folder for this item */
	TWeakPtr<FWaapiPlayerTreeItem> Parent;

	/** The row in the tree view associated to this item */
	ITableRow* TreeRow;

	/** Should this item be visible? */
	bool IsVisible;

	/** Constructor */
	FWaapiPlayerTreeItem(FString InDisplayName, FString InFolderPath, TSharedPtr<FWaapiPlayerTreeItem> InParent, EWaapiPlayerTreeItemType::Type InItemType)
		: DisplayName(MoveTemp(InDisplayName))
		, FolderPath(MoveTemp(InFolderPath))
		, ItemType(MoveTemp(InItemType))
		//, ItemId(InItemId)
		//, ChildCountInWwise(Children.Num())
        , Parent(MoveTemp(InParent))
        , IsVisible(true)
	{}

	/** Returns true if this item is a child of the specified item */
	bool IsChildOf(const FWaapiPlayerTreeItem& InParent)
	{
		auto CurrentParent = Parent.Pin();
		while (CurrentParent.IsValid())
		{
			if (CurrentParent.Get() == &InParent)
			{
				return true;
			}

			CurrentParent = CurrentParent->Parent.Pin();
		}

		return false;
	}

	/** Returns the child item by name or NULL if the child does not exist */
	TSharedPtr<FWaapiPlayerTreeItem> GetChild (const FString& InChildName)
	{
		for ( int32 ChildIdx = 0; ChildIdx < Children.Num(); ++ChildIdx )
		{
			if ( Children[ChildIdx]->DisplayName == InChildName )
			{
				return Children[ChildIdx];
			}
		}

		return TSharedPtr<FWaapiPlayerTreeItem>();
	}

	/** Finds the child who's path matches the one specified */
	TSharedPtr<FWaapiPlayerTreeItem> FindItemRecursive (const FString& InFullPath)
	{
		if ( InFullPath == FolderPath )
		{
			return SharedThis(this);
		}

		for ( int32 ChildIdx = 0; ChildIdx < Children.Num(); ++ChildIdx )
		{
			const TSharedPtr<FWaapiPlayerTreeItem>& Item = Children[ChildIdx]->FindItemRecursive(InFullPath);
			if ( Item.IsValid() )
			{
				return Item;
			}
		}

		return TSharedPtr<FWaapiPlayerTreeItem>(NULL);
	}

	struct FCompareWwiseTreeItem
	{
		FORCEINLINE bool operator()( TSharedPtr<FWaapiPlayerTreeItem> A, TSharedPtr<FWaapiPlayerTreeItem> B ) const
		{
			// Items are sorted like so:
			// 1- Physical folders, sorted alphabetically
			// 1- WorkUnits, sorted alphabetically
			// 2- Virtual folders, sorted alphabetically
			// 3- Normal items, sorted alphabetically
			if( A->ItemType == B->ItemType )
			{
				return A->DisplayName < B->DisplayName;
			}
			else if( A->ItemType == EWaapiPlayerTreeItemType::Folder )
			{
				return true;
			}
			else if( B->ItemType == EWaapiPlayerTreeItemType::Folder )
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	};

	/** Sort the children by name */
	void SortChildren ()
	{
		Children.Sort( FCompareWwiseTreeItem() );
	}
};
