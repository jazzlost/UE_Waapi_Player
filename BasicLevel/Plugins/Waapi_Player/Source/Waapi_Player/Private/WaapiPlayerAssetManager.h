#include "AkAudioEvent.h"
#include "Waapi_PlayerTreeItem.h"

class FAssetRegistryModule;

class WaapiPlayerAssetManager
{
public:
	static WaapiPlayerAssetManager& Get();

	void Init();
	void Clear();

	void SetEventBasePath(FString BasePath) { EventBasePath = BasePath; }
	FString GetEventBasePath() { return EventBasePath; }

public:
	TSharedPtr<FWaapiPlayerTreeItem> RootItem;
	TArray<TSharedPtr<FWaapiPlayerTreeItem>> ChildrenItems;

private:
	WaapiPlayerAssetManager();

	void Recurse(FString EventBasePath, FString BaseFolderName);

private:
	FAssetRegistryModule* AssetRegistryModule = nullptr;
	FString EventBasePath;
	FString EventBaseFolderName;

	TArray<UAkAudioEvent> AkEvents;

};