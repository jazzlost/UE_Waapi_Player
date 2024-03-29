#pragma once

#include "CoreMinimal.h"
#include "Toolkits/IToolkitHost.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "IWaapiPlayerAssetEditor.h"

class IDetailView;
class SDockableTab;
class UAkAudioEvent;
class UWaapiPlayerPlayingObject;
struct FWaapiEventObject;

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
	//virtual UAkAudioEvent* GetAsset();
	//virtual void SetAsset(UAkAudioEvent* Asset);
	virtual bool OnRequestClose() override;

	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* Asset, TSharedPtr<class FUICommandList> CommandList);
	bool GetIsInitDB() { return bInitDB; }

private:
	TSharedRef<SDockTab> SpawnAkEventTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTreeItemsTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnControlPanelTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTextItemsTab(const FSpawnTabArgs& Args);
	void RefreshTextItemsTab();
	void RefreshControlPanelTab();
	void RefreshAkEventTab();
	void RefreshPreplayingObject();

	void AddToolbarButton(TSharedPtr<class FUICommandList> EditorCommandList);
	void AddPlayButton(FToolBarBuilder& ToolbarBuilder);

	void InitWaapiSqlManager();
	void InitWaapiEventObject();
	void RegisterCallback();
	void QueryCallback(FString EventName);

private:
	TSharedPtr<class FUICommandList> EditorCommandList;

	TSharedPtr<SDockableTab> AkEventTab;
	TSharedPtr<SDockableTab> TreeItemsTab;
	TSharedPtr<SDockableTab> ControlPanelTab;
	TSharedPtr<SDockableTab> TextItemsTab;

	TSharedPtr<SVerticalBox> TextItemVerticalBox;
	TSharedPtr<SVerticalBox> ControlPanelVerticalBox;
	TSharedPtr<SVerticalBox> AkEventTabVerticalBox;

	TSharedPtr<class IDetailsView> AkEventDetailsView;

	static const FName AkEventTabId;
	static const FName TreeItemsTabId;
	static const FName ControlPanelTabId;
	static const FName TextItemsTabId;
	static const FName EditorAppIdentifier;

	UAkAudioEvent* SelectedAsset;

	bool bInitDB = false;
	TSharedPtr<FWaapiEventObject> OutResultObject;
};