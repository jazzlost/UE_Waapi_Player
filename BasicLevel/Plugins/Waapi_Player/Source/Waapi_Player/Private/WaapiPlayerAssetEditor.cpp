#include "WaapiPlayerAssetEditor.h"
#include "Modules/ModuleManager.h"
#include "EditorStyleSet.h"
#include "Widgets/Docking/SDockTab.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "Editor.h"
#include "AssetEditorToolkit.h"
#include "WaapiPlayer.h"

#define LOCTEXT_NAMESPACE "WaapiPlayerAssetEditor"

const FName FWaapiPlayerAssetEditor::ToolkitFName(TEXT("WaapiPlayerAssetEditor"));
const FName FWaapiPlayerAssetEditor::PropertiesTabId(TEXT("WaapiPlayerAssetEditor_Properties"));

void FWaapiPlayerAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager)
{
	auto WorkspaceMenuCategory = TabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_WaapiPlayerAssetEditor", "Waapi Player Asset Editor"));

	Super::RegisterTabSpawners(TabManager);
}

void FWaapiPlayerAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager)
{
}
