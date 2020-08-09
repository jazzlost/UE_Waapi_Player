#include "HAL/PlatformFilemanager.h"
#include "HAL/FileManager.h"
#include "Waapi_PlayerTreeItem.h"



class FWaapiPlayerDirectoryVisitor : public IPlatformFile::FDirectoryVisitor
{
public:
	virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override;

	void SetParentItem(TSharedPtr<FWaapiPlayerTreeItem> ParentItem) { ParentItemCache = ParentItem; }

	TSharedPtr<FWaapiPlayerTreeItem> CreateTreeItem(FString InDisplayName, FString InFolderPath, TSharedPtr<FWaapiPlayerTreeItem> InParent, EWaapiPlayerTreeItemType::Type InItemType);

private:

	bool VisitChild(FString FolderPath, TSharedPtr<FWaapiPlayerTreeItem> ParentItem);

	/**Store latest visited root item*/
	TSharedPtr<FWaapiPlayerTreeItem> ParentItemCache = nullptr;

};
