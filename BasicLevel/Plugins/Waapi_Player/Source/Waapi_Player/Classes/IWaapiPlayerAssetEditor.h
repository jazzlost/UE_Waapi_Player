#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"

class UAkAudioEvent;

class IWaapiPlayerAssetEditor : public FAssetEditorToolkit
{
public:

	virtual UAkAudioEvent* GetAsset() = 0;

	virtual void SetAsset(UAkAudioEvent* Asset) = 0;
};