#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"

class UAkAudioEvent;

DECLARE_MULTICAST_DELEGATE_OneParam(FWaapiPlayerTreeItemClickedCallback, FString)
DECLARE_MULTICAST_DELEGATE_OneParam(FWaapiPlayerEventClickedCallback, FString)

class IWaapiPlayerAssetEditor : public FAssetEditorToolkit
{
public:

	FWaapiPlayerTreeItemClickedCallback OnTreeItemSelected;
	FWaapiPlayerEventClickedCallback OnEventSelected;
};