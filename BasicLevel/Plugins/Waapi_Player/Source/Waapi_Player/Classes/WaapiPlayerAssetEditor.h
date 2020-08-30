#pragma once

#include "CoreMinimal.h"
#include "Toolkits/IToolkitHost.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "IWaapiPlayerAssetEditor.h"

class IDetailView;
class SDockableTab;
class UAkAudioEvent;

class WAAPIPLAYER_API FWaapiPlayerAssetEditor : public IWaapiPlayerAssetEditor
{
public:
	/** IToolkit interface */
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;

	virtual FName GetToolkitFName() const override;				// Must implement in derived class!
	virtual FText GetBaseToolkitName() const override;			// Must implement in derived class!
	virtual FString GetWorldCentricTabPrefix() const override;	// Must implement in derived class!
	virtual bool IsPrimaryEditor() const override { return true; }
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual UAkAudioEvent* GetAsset();
	virtual void SetAsset(UAkAudioEvent* Asset);

	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* Asset, TSharedPtr<class FUICommandList> CommandList);

private:
	TSharedRef<SDockTab> SpawnAkEventTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTreeItemsTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnControlPanelTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTextItemsTab(const FSpawnTabArgs& Args);

	void AddToolbarButton(TSharedPtr<class FUICommandList> EditorCommandList);
	void AddPlayButton(FToolBarBuilder& ToolbarBuilder);

private:
	TSharedPtr<class FUICommandList> EditorCommandList;

	TSharedPtr<SDockableTab> AkEventTab;
	TSharedPtr<SDockableTab> TreeItemsTab;
	TSharedPtr<SDockableTab> ControlPanelTab;
	TSharedPtr<SDockableTab> TextItemsTab;

	TSharedPtr<class IDetailsView> AkEventDetailsView;


	static const FName AkEventTabId;
	static const FName TreeItemsTabId;
	static const FName ControlPanelTabId;
	static const FName TextItemsTabId;
	static const FName EditorAppIdentifier;

	UAkAudioEvent* Asset;

};