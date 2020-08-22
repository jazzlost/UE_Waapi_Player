#include "WaapiPlayerAssetEditor.h"
#include "Modules/ModuleManager.h"
#include "EditorStyleSet.h"
#include "Widgets/Docking/SDockTab.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "Editor.h"
#include "AssetEditorToolkit.h"
#include "WaapiPlayer.h"
#include "SPlayerControlPanelWidget.h"

#define LOCTEXT_NAMESPACE "WaapiPlayerAssetEditor"

const FName FWaapiPlayerAssetEditor::AkEventTabId(TEXT("WaapiPlayerAssetEditor_AkEvent"));
const FName FWaapiPlayerAssetEditor::TreeItemsTabId(TEXT("WaapiPlayerAssetEditor_TreeItems"));
const FName FWaapiPlayerAssetEditor::ControlPanelTabId(TEXT("WaapiPlayerAssetEditor_ControlPanel"));


void FWaapiPlayerAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager)
{
	WorkspaceMenuCategory = TabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_WaapiPlayerAssetEditor", "Waapi Player Asset Editor"));

	FAssetEditorToolkit::RegisterTabSpawners(TabManager);

	TabManager->RegisterTabSpawner(AkEventTabId, FOnSpawnTab::CreateSP(this, &FWaapiPlayerAssetEditor::SpawnAkEventTab))
		.SetDisplayName(LOCTEXT("AkEventTabTitle", "AkEvent"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	TabManager->RegisterTabSpawner(TreeItemsTabId, FOnSpawnTab::CreateSP(this, &FWaapiPlayerAssetEditor::SpawnTreeItemsTab))
		.SetDisplayName(LOCTEXT("TreeItemsTabTitle", "TreeItems"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	TabManager->RegisterTabSpawner(ControlPanelTabId, FOnSpawnTab::CreateSP(this, &FWaapiPlayerAssetEditor::SpawnControlPanelTab))
		.SetDisplayName(LOCTEXT("ControlPanelTabTitle", "ControlPanel"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

}

void FWaapiPlayerAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(TabManager);

	TabManager->UnregisterTabSpawner(AkEventTabId);
	TabManager->UnregisterTabSpawner(TreeItemsTabId);
	TabManager->UnregisterTabSpawner(ControlPanelTabId);
}

FName FWaapiPlayerAssetEditor::GetToolkitFName() const
{
	return FName("WaapiPlayerAssetEditor");
}

FText FWaapiPlayerAssetEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "Waapi Player Asset Editor");
}


FString FWaapiPlayerAssetEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "WaapiPlayerAssetEditor").ToString();
}

FLinearColor FWaapiPlayerAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.5f, 0.0f, 0.0f, 0.5f);
}

TSharedRef<SDockTab> FWaapiPlayerAssetEditor::SpawnAkEventTab(const FSpawnTabArgs & Args)
{
	return TSharedRef<SDockTab>();
}

TSharedRef<SDockTab> FWaapiPlayerAssetEditor::SpawnTreeItemsTab(const FSpawnTabArgs & Args)
{
	return TSharedRef<SDockTab>();

}

TSharedRef<SDockTab> FWaapiPlayerAssetEditor::SpawnControlPanelTab(const FSpawnTabArgs & Args)
{
	check(Args.GetTabId == ControlPanelTabId);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("GenericEditor.Tab.Properties"))
		.Label(LOCTEXT("WaapiPlayerAssetEditorLabel", "Control Panel"))
		.TabColorScale(GetTabColorScale())
		[
			SNew(SPlayerControlPanelWidget)
		]
}
