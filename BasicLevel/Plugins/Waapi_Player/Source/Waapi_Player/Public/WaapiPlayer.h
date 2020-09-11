// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IToolkit.h"

class FToolBarBuilder;
class FMenuBuilder;
class IWaapiPlayerAssetEditor;
class FWaapiPlayerAssetEditor;
class UWaapiPlayerPlayingObject;

class FWaapiPlayerModule : public IModuleInterface, public IHasToolBarExtensibility, public IHasMenuExtensibility
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */

	TSharedPtr<FWaapiPlayerAssetEditor> GetAssetEditor() { return MyAssetEditor; }

	virtual TSharedRef<FWaapiPlayerAssetEditor> CreateAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* Asset);
	
	virtual TSharedPtr<FExtensibilityManager> GetMenuExtensibilityManager() override { return MenuExtensibilityManager; }

	virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() override { return ToolBarExtensibilityManager; }

	void PluginButtonClicked();
	void OnPlayButtonPressed();

	static UWaapiPlayerPlayingObject* GetPreplayingObject();

private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);


	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
	void CreateAssetEditorWrapped();

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;
	TSharedPtr<FExtensibilityManager> MenuExtensibilityManager;
	TSharedPtr<FWaapiPlayerAssetEditor> MyAssetEditor;

	static UWaapiPlayerPlayingObject* PreplayingObject;

};
