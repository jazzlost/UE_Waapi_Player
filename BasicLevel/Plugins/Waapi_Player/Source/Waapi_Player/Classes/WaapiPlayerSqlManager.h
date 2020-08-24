#pragma once

#include "CoreMinimal.h"

struct WaapiSwitchStateObject
{
	FString SwitchOrStateGroup;
	TArray<TSharedPtr<FName>> SwitchOrState;
	bool bSwitch;
};

struct WaapiRtpcObject
{
	FString RtpcName;
	float DefaultValue;
	float MaxValue;
	float MinValue;
};

struct WaapiTargetObject
{
	FString TargetName;
	float Volume;
	int Pitch;
	int LPF;
	int HPF;
	bool UseMaxSoundInstance;
	int MaxSound;
	bool UseListenerRelativeRoute;
	FString Spatialization3D;
	int MaxDistance;
	bool UseConeAttenuation;
};

struct WaapiEventObject
{
	FString EventName;
	TMap<int, WaapiTargetObject> Targets;
};

struct WaapiQuaryObject
{
	FString EventName;
};

struct WaapiDataObject
{
	TArray<WaapiSwitchStateObject> SwitchOrStateObjects;
	TArray<WaapiRtpcObject> RtpcObjects;
};