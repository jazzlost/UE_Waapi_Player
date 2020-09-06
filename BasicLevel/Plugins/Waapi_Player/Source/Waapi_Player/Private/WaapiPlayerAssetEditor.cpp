#include "WaapiPlayerAssetEditor.h"
#include "Modules/ModuleManager.h"
#include "EditorStyleSet.h"
#include "Widgets/Docking/SDockTab.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "Editor.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "AssetEditorToolkit.h"
#include "WaapiPlayer.h"
#include "SPlayerControlPanelWidget.h"
#include "SPlayerTreeViewWidget.h"
#include "SPlayerTextViewWidget.h"
#include "AkAudioEvent.h"
#include "WaapiTargetObject.h"
#include "WaapiPlayerCommands.h"
#include "WaapiPlayerSqlManager.h"
#include "Misc/Paths.h"



#define LOCTEXT_NAMESPACE "WaapiPlayerAssetEditor"

const FName FWaapiPlayerAssetEditor::AkEventTabId(TEXT("WaapiPlayerAssetEditor_AkEvent"));
const FName FWaapiPlayerAssetEditor::TreeItemsTabId(TEXT("WaapiPlayerAssetEditor_TreeItems"));
const FName FWaapiPlayerAssetEditor::ControlPanelTabId(TEXT("WaapiPlayerAssetEditor_ControlPanel"));
const FName FWaapiPlayerAssetEditor::TextItemsTabId(TEXT("WaapiPlayerAssetEditor_TextItems"));
const FName FWaapiPlayerAssetEditor::EditorAppIdentifier(TEXT("WaapiPlayerAssetEditorApp"));

const FString DatabaseRelativePath = TEXT("/Waapi_Player/Content/waapi.db");


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

	TabManager->RegisterTabSpawner(TextItemsTabId, FOnSpawnTab::CreateSP(this, &FWaapiPlayerAssetEditor::SpawnTextItemsTab))
		.SetDisplayName(LOCTEXT("TextItemsTabTitle", "TextItems"))
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
	TabManager->UnregisterTabSpawner(TextItemsTabId);
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

//UAkAudioEvent * FWaapiPlayerAssetEditor::GetAsset()
//{
//	return nullptr;
//}
//
//void FWaapiPlayerAssetEditor::SetAsset(UAkAudioEvent * Asset)
//{
//}

bool FWaapiPlayerAssetEditor::OnRequestClose()
{
	WaapiPlaySqlManager::Get().Close();

	return true;
}

void FWaapiPlayerAssetEditor::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject * Asset, TSharedPtr<class FUICommandList> CommandList)
{
	WaapiPlaySqlManager::Get().Close();

	const bool bIsUpdatable = false;
	const bool bAllowFavorites = true;
	const bool bIsLockable = false;
	EditorCommandList = CommandList;

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs(bIsUpdatable, bIsLockable, true, FDetailsViewArgs::ObjectsUseNameArea, false);
	AkEventDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	AkEventDetailsView->SetObject(UAkAudioEvent::StaticClass()->GetDefaultObject());

	AddToolbarButton(EditorCommandList);


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
					->AddTab(TextItemsTabId, ETabState::OpenedTab)
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
	
	InitWaapiSqlManager();
	RegisterCallback();
}

TSharedRef<SDockTab> FWaapiPlayerAssetEditor::SpawnAkEventTab(const FSpawnTabArgs & Args)
{
	check(Args.GetTabId() == AkEventTabId);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("GenericEditor.Tab.Properties"))
		.Label(LOCTEXT("WaapiPlayerAssetEditorLabel", "AkAudioEvent"))
		.TabColorScale(GetTabColorScale())
		[
			AkEventDetailsView.ToSharedRef()
		];
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

TSharedRef<SDockTab> FWaapiPlayerAssetEditor::SpawnTextItemsTab(const FSpawnTabArgs & Args)
{
	TSharedPtr<FWaapiEventObject> WaapiNewObject = MakeShareable(new FWaapiEventObject);
	//WaapiNewObject->EventName = TEXT("WaapiEventObjectName");
	//UWaapiTargetObject* Target = NewObject<UWaapiTargetObject>();
	//Target->TargetName = TEXT("TargetName");
	//Target->Volume = TEXT("20");
	//Target->Pitch = TEXT("30");
	//Target->LPF = TEXT("1200");
	//Target->HPF = TEXT("5000");
	//Target->UseMaxSoundInstance = TEXT("yes");
	//Target->MaxSound = TEXT("5");
	//Target->UseListenerRelativeRoute = TEXT("yes");
	//Target->Spatialization3D = TEXT("Relative");
	//Target->MaxDistance = TEXT("10000");
	//Target->UseConeAttenuation = TEXT("no");
	//
	//WaapiNewObject->Targets.Add(Target);

	//UWaapiTargetObject* Target02 = NewObject<UWaapiTargetObject>();
	//Target02->TargetName = TEXT("TargetName");
	//Target02->Volume = TEXT("20");
	//Target02->Pitch = TEXT("30");
	//Target02->LPF = TEXT("1200");
	//Target02->HPF = TEXT("5000");
	//Target02->UseMaxSoundInstance = TEXT("yes");
	//Target02->MaxSound = TEXT("5");
	//Target02->UseListenerRelativeRoute = TEXT("yes");
	//Target02->Spatialization3D = TEXT("Relative");
	//Target02->MaxDistance = TEXT("10000");
	//Target02->UseConeAttenuation = TEXT("no");

	//WaapiNewObject->Targets.Add(Target02);



	check(Args.GetTabId() == TextItemsTabId);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("GenericEditor.Tab.Properties"))
		.Label(LOCTEXT("WaapiPlayerAssetEditorLabel", "Event Info"))
		.TabColorScale(GetTabColorScale())
		[
			SNew(SPlayerTextViewWidget)
			.WaapiEventObject(WaapiNewObject)
		];
}

void FWaapiPlayerAssetEditor::AddToolbarButton(TSharedPtr<class FUICommandList> EditorCommandList)
{
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender());
	ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After, EditorCommandList, FToolBarExtensionDelegate::CreateRaw(this, &FWaapiPlayerAssetEditor::AddPlayButton));
	AddToolbarExtender(ToolbarExtender);
}

void FWaapiPlayerAssetEditor::AddPlayButton(FToolBarBuilder& ToolbarBuilder)
{
	ToolbarBuilder.AddToolBarButton(FWaapiPlayerCommands::Get().PressPlayButton, FName(TEXT("PlayButton")), LOCTEXT("PlayButtonName", "Play"));
}

void FWaapiPlayerAssetEditor::InitWaapiSqlManager()
{
	FString DatabasePath = FPaths::ProjectPluginsDir() + DatabaseRelativePath;
	bInitDB = WaapiPlaySqlManager::Get().Init(DatabasePath);
}

void FWaapiPlayerAssetEditor::RegisterCallback()
{
	OnTreeItemSelected.AddRaw(this, &FWaapiPlayerAssetEditor::QueryCallback);
}

void FWaapiPlayerAssetEditor::QueryCallback(FString EventName)
{
	OutResultObject = MakeShareable(new FWaapiEventObject);
	if (bInitDB)
	{
		WaapiPlaySqlManager::Get().QueryWaapiTargetObjects(EventName, OutResultObject);
	}
}
