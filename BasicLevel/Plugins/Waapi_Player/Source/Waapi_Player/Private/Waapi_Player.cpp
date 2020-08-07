// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Waapi_Player.h"
#include "Waapi_PlayerStyle.h"
#include "Waapi_PlayerCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "SPlayerMainWidget.h"

static const FName Waapi_PlayerTabName("Waapi_Player");

#define LOCTEXT_NAMESPACE "FWaapi_PlayerModule"

void FWaapi_PlayerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FWaapi_PlayerStyle::Initialize();
	FWaapi_PlayerStyle::ReloadTextures();

	FWaapi_PlayerCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FWaapi_PlayerCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FWaapi_PlayerModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FWaapi_PlayerModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FWaapi_PlayerModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(Waapi_PlayerTabName, FOnSpawnTab::CreateRaw(this, &FWaapi_PlayerModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FWaapi_PlayerTabTitle", "Waapi_Player"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FWaapi_PlayerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FWaapi_PlayerStyle::Shutdown();

	FWaapi_PlayerCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(Waapi_PlayerTabName);
}

TSharedRef<SDockTab> FWaapi_PlayerModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FWaapi_PlayerModule::OnSpawnPluginTab")),
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
					SNew(SPlayerMainWidget)
				]
			//]
		];
}

void FWaapi_PlayerModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(Waapi_PlayerTabName);
}

void FWaapi_PlayerModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FWaapi_PlayerCommands::Get().OpenPluginWindow);
}

void FWaapi_PlayerModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FWaapi_PlayerCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWaapi_PlayerModule, Waapi_Player)