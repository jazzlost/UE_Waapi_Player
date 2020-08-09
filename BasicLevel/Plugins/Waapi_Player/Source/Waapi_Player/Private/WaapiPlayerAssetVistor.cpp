#include "WaapiPlayerAssetVistor.h"
#include "WaapiPlayerAssetManager.h"
#include "Misc/Paths.h"


bool FWaapiPlayerDirectoryVisitor::Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory)
{
	WaapiPlayerAssetManager& AssetManager = WaapiPlayerAssetManager::Get();
	TSharedPtr<FWaapiPlayerTreeItem> TreeItem = nullptr;

	FString FolderPathStr = FilenameOrDirectory;
	FString FolderName = FPaths::GetPathLeaf(FilenameOrDirectory);

	if (bIsDirectory)
	{
		TreeItem = CreateTreeItem(FolderName, FolderPathStr, ParentItemCache, EWaapiPlayerTreeItemType::Type::Folder);
		if (ParentItemCache.IsValid())
		{
			ParentItemCache->Children.Add(TreeItem);
		}

		VisitChild(FolderPathStr, TreeItem);
	}
	else
	{
		TreeItem = CreateTreeItem(FolderName, FolderPathStr, ParentItemCache, EWaapiPlayerTreeItemType::Type::Event);
		if (ParentItemCache.IsValid())
		{
			ParentItemCache->Children.Add(TreeItem);
		}
	}

	return true;
}

TSharedPtr<FWaapiPlayerTreeItem> FWaapiPlayerDirectoryVisitor::CreateTreeItem(FString InDisplayName, FString InFolderPath, TSharedPtr<FWaapiPlayerTreeItem> InParent, EWaapiPlayerTreeItemType::Type InItemType)
{
	return MakeShareable(new FWaapiPlayerTreeItem(InDisplayName, InFolderPath, InParent, InItemType));
}

bool FWaapiPlayerDirectoryVisitor::VisitChild(FString FolderPath, TSharedPtr<FWaapiPlayerTreeItem> ParentItem)
{
	auto& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FWaapiPlayerDirectoryVisitor DirectoryVisitor;
	DirectoryVisitor.SetParentItem(ParentItem);

	PlatformFile.IterateDirectory(*FolderPath, DirectoryVisitor);

	return true;
}

