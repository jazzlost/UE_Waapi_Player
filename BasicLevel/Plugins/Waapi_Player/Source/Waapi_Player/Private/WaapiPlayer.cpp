// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "WaapiPlayer.h"
#include "WaapiPlayerStyle.h"
#include "WaapiPlayerCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "SPlayerTreeViewWidget.h"
#include "SPlayerControlPanelWidget.h"

static const FName Waapi_PlayerTabName("Waapi_Player");

#define LOCTEXT_NAMESPACE "FWaapiPlayerModule"

void FWaapiPlayerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FWaapiPlayerStyle::Initialize();
	FWaapiPlayerStyle::ReloadTextures();

	FWaapiPlayerCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FWaapiPlayerCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FWaapiPlayerModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FWaapiPlayerModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FWaapiPlayerModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(Waapi_PlayerTabName, FOnSpawnTab::CreateRaw(this, &FWaapiPlayerModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FWaapi_PlayerTabTitle", "Waapi_Player"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FWaapiPlayerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FWaapiPlayerStyle::Shutdown();

	FWaapiPlayerCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(Waapi_PlayerTabName);
}

TSharedRef<SDockTab> FWaapiPlayerModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FWaapiPlayerModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("Waapi_Player.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			//SNew(SBox)
			//.HAlign(HAlign_Center)
			//.VAlign(VAlign_Center)
			//[
				/*SNew(STextBlock)
				.Text(WidgetText)*/
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SPlayerTreeViewWidget)
				]
				
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SPlayerControlPanelWidget)
				]
			//]
		];
}

void FWaapiPlayerModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(Waapi_PlayerTabName);
}

void FWaapiPlayerModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FWaapiPlayerCommands::Get().OpenPluginWindow);
}

void FWaapiPlayerModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FWaapiPlayerCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWaapiPlayerModule, Waapi_Player)