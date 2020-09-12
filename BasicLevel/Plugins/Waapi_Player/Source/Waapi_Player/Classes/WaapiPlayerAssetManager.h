#pragma once

#include "AkAudioEvent.h"
#include "WaapiPlayerTreeItem.h"

DECLARE_MULTICAST_DELEGATE(FAssetManagerInitDelegate)

class FAssetRegistryModule;

class WaapiPlayerAssetManager
{
public:
	static WaapiPlayerAssetManager& Get();

	void Init();
	void Clear();

	void SetEventBasePath(FString BasePath) { EventBasePath = BasePath; }
	FString GetEventBasePath() { return EventBasePath; }

	UAkAudioEvent* GetAkEventObjectByName(FName Name);

public:
	FAssetManagerInitDelegate AssetManagerInitedCallback;

	TSharedPtr<FWaapiPlayerTreeItem> RootItem;
	//TArray<TSharedPtr<FWaapiPlayerTreeItem>> ChildrenItems;

private:
	WaapiPlayerAssetManager();

	void Recurse(FString EventBasePath, FString BaseFolderName);

private:
	FAssetRegistryModule* AssetRegistryModule = nullptr;
	FString EventBasePath;
	FString EventBaseFolderName;

	TArray<UAkAudioEvent> AkEvents;

};