#include "WaapiPlayerAssetManager.h"
#include "WaapiPlayerAssetVistor.h"

#include "AssetRegistry/Public/AssetRegistryModule.h"
#include "AssetTools/Public/AssetToolsModule.h"
//#include "Async/Async.h"
//#include "ContentBrowser/Public/ContentBrowserModule.h"
//#include "ContentBrowser/Public/IContentBrowserSingleton.h"
//#include "HAL/FileManager.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/ScopedSlowTask.h"

#include "WaapiPlayerSqlManager.h"


#define LOCTEXT_NAMESPACE "WaapiPlayer"

WaapiPlayerAssetManager & WaapiPlayerAssetManager::Get()
{
	static WaapiPlayerAssetManager instance;
	return instance;
}


WaapiPlayerAssetManager::WaapiPlayerAssetManager()
{
	AssetRegistryModule = &FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	EventBasePath = TEXT("/Game/WwiseEvent");
	EventBaseFolderName = TEXT("WwiseEvent");
}

void WaapiPlayerAssetManager::Init()
{
	AkEvents.Empty();
	ChildrenItems.Empty();

	Recurse(EventBasePath, EventBaseFolderName);

	AssetManagerInitedCallback.Broadcast();
}

void WaapiPlayerAssetManager::Clear()
{

}

UAkAudioEvent * WaapiPlayerAssetManager::GetAkEventObjectByName(FName Name)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetData;
	FARFilter Filter;
	Filter.bRecursivePaths = true;
	Filter.ClassNames.Add(FName("AkAudioEvent"));
	Filter.PackagePaths.Add("/Game/WwiseEvent/");
	AssetRegistryModule.Get().GetAssets(Filter, AssetData);
	if (AssetData.Num() > 0)
	{
		for (auto Asset : AssetData)
		{
			if (Asset.AssetName == Name)
			{
				return Cast<UAkAudioEvent>(Asset.GetAsset());
			}
		}
	}

	return nullptr;
}


void WaapiPlayerAssetManager::Recurse(FString EventBasePath, FString BaseFolderName)
{
	auto BasePath = FPackageName::LongPackageNameToFilename(EventBasePath);


	auto& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FWaapiPlayerDirectoryVisitor DirectoryVisitor;

	RootItem = DirectoryVisitor.CreateTreeItem(BaseFolderName, EventBasePath, nullptr, EWaapiPlayerTreeItemType::Type::Event);
	DirectoryVisitor.SetParentItem(RootItem);

	//DirectoryVisitor.SetIsRootVisit(true);
	PlatformFile.IterateDirectory(*BasePath, DirectoryVisitor);

	//DirectoryVisitor.SetIsRootVisit(false);
	//for (auto RootItem : RootItems)
	//{
	//	PlatformFile.IterateDirectory(*(RootItem->FolderPath), DirectoryVisitor);
	//}


	//TArray<FAssetData> Assets;
	//auto& AssetRegistry = AssetRegistryModule->Get();

	//AssetRegistry.GetAssetsByPath(FName(*Path), Assets, true);

	//FScopedSlowTask SlowTask(static_cast<float>(Assets.Num()), LOCTEXT("WaapiPlayer_ScanAssets", "Scanning Ak Events"));
	//SlowTask.MakeDialog();

	//for (auto& Asset : Assets)
	//{
	//	FString Leaf = FPaths::GetPathLeaf(Asset.PackagePath.ToString());

	//	FString Message = FString::Printf(TEXT("Scanning Ak Event: %s"), *Asset.AssetName.ToString());
	//	SlowTask.EnterProgressFrame(1.0f, FText::FromString(Message));
	//}


	//for (auto ChildItem : ChildrenItems)
	//{

	//}

	//TArray<FAssetData> Assets;
	//auto& AssetRegistry = AssetRegistryModule->Get();

}
