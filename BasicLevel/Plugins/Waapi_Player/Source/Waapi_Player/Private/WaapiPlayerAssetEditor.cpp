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
#include "SPlayerTreeViewWidget.h"
#include "AkAudioEvent.h"

#define LOCTEXT_NAMESPACE "WaapiPlayerAssetEditor"

const FName FWaapiPlayerAssetEditor::AkEventTabId(TEXT("WaapiPlayerAssetEditor_AkEvent"));
const FName FWaapiPlayerAssetEditor::TreeItemsTabId(TEXT("WaapiPlayerAssetEditor_TreeItems"));
const FName FWaapiPlayerAssetEditor::ControlPanelTabId(TEXT("WaapiPlayerAssetEditor_ControlPanel"));
const FName FWaapiPlayerAssetEditor::EditorAppIdentifier(TEXT("WaapiPlayerAssetEditorApp"));


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

UAkAudioEvent * FWaapiPlayerAssetEditor::GetAsset()
{
	return nullptr;
}

void FWaapiPlayerAssetEditor::SetAsset(UAkAudioEvent * Asset)
{
}

void FWaapiPlayerAssetEditor::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject * Asset)
{
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_WaapiPlayerAssetEditor_Layout");
	StandaloneDefaultLayout
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->SetHideTabWell(true)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewSplitter()
				->Split
				(
					FTabManager::NewStack()
					->AddTab(AkEventTabId, ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(TreeItemsTabId, ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(ControlPanelTabId, ETabState::OpenedTab)
				)
			)
		);

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, FWaapiPlayerAssetEditor::EditorAppIdentifier, StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, Asset);
}

TSharedRef<SDockTab> FWaapiPlayerAssetEditor::SpawnAkEventTab(const FSpawnTabArgs & Args)
{
	return SNew(SDockTab);
}

TSharedRef<SDockTab> FWaapiPlayerAssetEditor::SpawnTreeItemsTab(const FSpawnTabArgs & Args)
{
	check(Args.GetTabId() == TreeItemsTabId);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("GenericEditor.Tab.Properties"))
		.Label(LOCTEXT("WaapiPlayerAssetEditorLabel", "Event TreeView"))
		.TabColorScale(GetTabColorScale())
		[
			SNew(SPlayerTreeViewWidget)
		];
}

TSharedRef<SDockTab> FWaapiPlayerAssetEditor::SpawnControlPanelTab(const FSpawnTabArgs & Args)
{
	check(Args.GetTabId() == ControlPanelTabId);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("GenericEditor.Tab.Properties"))
		.Label(LOCTEXT("WaapiPlayerAssetEditorLabel", "Control Panel"))
		.TabColorScale(GetTabColorScale())
		[
			SNew(SPlayerControlPanelWidget)
		];
}
