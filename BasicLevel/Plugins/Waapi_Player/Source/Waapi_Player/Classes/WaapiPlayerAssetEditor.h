#pragma once

#include "CoreMinimal.h"
#include "Toolkits/IToolkitHost.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "IWaapiPlayerAssetEditor.h"

class IDetailView;
class SDockableTab;
class UAkAudioEvent;

class FWaapiPlayerAssetEditor : public IWaapiPlayerAssetEditor
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

	void InitAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UAkAudioEvent * Asset);

private:
	TSharedRef<SDockTab> SpawnAkEventTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTreeItemsTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnControlPanelTab(const FSpawnTabArgs& Args);

	TSharedPtr<SDockableTab> AkEventTab;
	TSharedPtr<SDockableTab> TreeItemsTab;
	TSharedPtr<SDockableTab> ControlPanelTab;

	TSharedPtr<IDetailView> DetailsView;

	static const FName AkEventTabId;
	static const FName TreeItemsTabId;
	static const FName ControlPanelTabId;

	UAkAudioEvent* Asset;

};