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

	virtual FName GetToolkitFName() const override = 0;				// Must implement in derived class!
	virtual FText GetBaseToolkitName() const override = 0;			// Must implement in derived class!
	virtual FText GetToolkitToolTipText() const override;
	virtual FString GetWorldCentricTabPrefix() const override = 0;	// Must implement in derived class!
	virtual bool IsPrimaryEditor() const override { return true; }

	virtual UAkAudioEvent* GetAsset();
	virtual void SetAsset(UAkAudioEvent* Asset);

private:
	TSharedRef<SDockTab> SpawnPropertiesTab(const FSpawnTabArgs& Args);

	TSharedPtr<SDockableTab> PropertiesTab;

	TSharedPtr<IDetailView> DetailsView;

	static const FName PropertiesTabId;

	UAkAudioEvent* Asset;

};