#include "WaapiPlayerAssetEditor.h"
#include "Modules/ModuleManager.h"
#include "EditorStyleSet.h"
#include "Widgets/Docking/SDockTab.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "Editor.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "AssetEditorToolkit.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Misc/Paths.h"
#include "WaapiPlayer.h"
#include "SPlayerControlPanelWidget.h"
#include "SPlayerTreeViewWidget.h"
#include "SPlayerTextViewWidget.h"
#include "AkAudioEvent.h"
#include "WaapiTargetObject.h"
#include "WaapiPlayerCommands.h"
#include "WaapiPlayerSqlManager.h"
#include "WaapiPlayerAssetManager.h"
#include "WaapiPlayerPlayingObject.h"



#define LOCTEXT_NAMESPACE "WaapiPlayerAssetEditor"

const FName FWaapiPlayerAssetEditor::AkEventTabId(TEXT("WaapiPlayerAssetEditor_AkEvent"));
const FName FWaapiPlayerAssetEditor::TreeItemsTabId(TEXT("WaapiPlayerAssetEditor_TreeItems"));
const FName FWaapiPlayerAssetEditor::ControlPanelTabId(TEXT("WaapiPlayerAssetEditor_ControlPanel"));
const FName FWaapiPlayerAssetEditor::TextItemsTabId(TEXT("WaapiPlayerAssetEditor_TextItems"));
const FName FWaapiPlayerAssetEditor::EditorAppIdentifier(TEXT("WaapiPlayerAssetEditorApp"));

//const FString DatabaseRelativePath = TEXT("/Waapi_Player/Content/waapi.db");
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

	const bool bIsUpdatable = true;
	const bool bAllowFavorites = true;
	const bool bIsLockable = false;
	EditorCommandList = CommandList;

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs(bIsUpdatable, bIsLockable, true, FDetailsViewArgs::ObjectsUseNameArea, false);
	AkEventDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	if (Asset == nullptr)
	{
		AkEventDetailsView->SetObject(UAkAudioEvent::StaticClass()->GetDefaultObject());
	}
	else
	{
		SelectedAsset = Cast<UAkAudioEvent>(Asset);
		if (SelectedAsset)
		{
			AkEventDetailsView->SetObject(SelectedAsset);
		}
	}

	AddToolbarButton(EditorCommandList);
	InitWaapiEventObject();

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
			SAssignNew(AkEventTabVerticalBox, SVerticalBox)

			+ SVerticalBox::Slot()
			[
				AkEventDetailsView.ToSharedRef()
			]

		];
}

TSharedRef<SDockTab> FWaapiPlayerAssetEditor::SpawnTreeItemsTab(const FSpawnTabArgs & Args)
{
	check(Args.GetTabId() == TreeItemsTabId);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("GenericEditor.Tab.Properties"))
		.Label(LOCTEXT("WaapiPlayerAssetEditorLabel", "Event TreeView"))
		.TabRole(ETabRole::NomadTab)
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
			SAssignNew(ControlPanelVerticalBox, SVerticalBox)

			+SVerticalBox::Slot()
			[
				SNew(SPlayerControlPanelWidget)
				.TargetObjects(OutResultObject->Targets)
			]
		];
}

TSharedRef<SDockTab> FWaapiPlayerAssetEditor::SpawnTextItemsTab(const FSpawnTabArgs & Args)
{
	check(Args.GetTabId() == TextItemsTabId);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("GenericEditor.Tab.Properties"))
		.Label(LOCTEXT("WaapiPlayerAssetEditorLabel", "Event Info"))
		.TabColorScale(GetTabColorScale())
		[
			SAssignNew(TextItemVerticalBox, SVerticalBox)

			+SVerticalBox::Slot()
			[
				SNew(SPlayerTextViewWidget)
				.WaapiEventObject(OutResultObject)
			]
		];
}

void FWaapiPlayerAssetEditor::RefreshTextItemsTab()
{
	TextItemVerticalBox->ClearChildren();
	TextItemVerticalBox->AddSlot()
	[
		SNew(SPlayerTextViewWidget)
		.WaapiEventObject(OutResultObject)
	];
}

void FWaapiPlayerAssetEditor::RefreshControlPanelTab()
{
	ControlPanelVerticalBox->ClearChildren();
	ControlPanelVerticalBox->AddSlot()
	[
		SNew(SPlayerControlPanelWidget)
		.TargetObjects(OutResultObject->Targets)
	];
}

void FWaapiPlayerAssetEditor::RefreshAkEventTab()
{
	AkEventDetailsView->SetObject(SelectedAsset);

	AkEventTabVerticalBox->ClearChildren();
	AkEventTabVerticalBox->AddSlot()
	[
		AkEventDetailsView.ToSharedRef()
	];
}

void FWaapiPlayerAssetEditor::RefreshPreplayingObject()
{
	FWaapiPlayerModule::GetPreplayingObject()->EventName = SelectedAsset->GetName();
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

void FWaapiPlayerAssetEditor::InitWaapiEventObject()
{
	OutResultObject = MakeShareable(new FWaapiEventObject);
	UWaapiTargetObject* DefaultTarget = NewObject<UWaapiTargetObject>();
	
	TSharedPtr<WaapiSwitchStateObject> DefaultSwitch = MakeShareable(new WaapiSwitchStateObject);
	TSharedPtr<FName> DefaultSwitchState = MakeShareable(new FName(TEXT("None")));
	DefaultSwitch->SwitchOrState.Add(DefaultSwitchState);
	DefaultTarget->TargetExtraData.SwitchOrStateObjects.Add(DefaultSwitch);

	TSharedPtr<WaapiRtpcObject> DefaultRtpc = MakeShareable(new WaapiRtpcObject);
	DefaultTarget->TargetExtraData.RtpcObjects.Add(DefaultRtpc);

	OutResultObject->Targets.Add(DefaultTarget);
}

void FWaapiPlayerAssetEditor::RegisterCallback()
{
	OnTreeItemSelected.AddRaw(this, &FWaapiPlayerAssetEditor::QueryCallback);
	OnEventSelected.AddRaw(this, &FWaapiPlayerAssetEditor::QueryCallback);
}

void FWaapiPlayerAssetEditor::QueryCallback(FString EventName)
{
	OutResultObject = MakeShareable(new FWaapiEventObject);
	if (bInitDB)
	{
		WaapiPlaySqlManager::Get().QueryWaapiTargetObjects(EventName, OutResultObject);
		RefreshTextItemsTab();
		RefreshControlPanelTab();

		UAkAudioEvent* TempEvent = WaapiPlayerAssetManager::Get().GetAkEventObjectByName(FName(*EventName));
		if (TempEvent)
		{
			SelectedAsset = TempEvent;
		}
		else
		{
			SelectedAsset = Cast<UAkAudioEvent>(UAkAudioEvent::StaticClass()->GetDefaultObject());
		}

		RefreshAkEventTab();
		RefreshPreplayingObject();
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Waapi Database Open Failed!")));
	}
}
