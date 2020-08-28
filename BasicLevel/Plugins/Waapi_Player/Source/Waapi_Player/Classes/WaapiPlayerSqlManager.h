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
	FString Volume;
	FString Pitch;
	FString LPF;
	FString HPF;
	FString UseMaxSoundInstance;
	FString MaxSound;
	FString UseListenerRelativeRoute;
	FString Spatialization3D;
	FString MaxDistance;
	FString UseConeAttenuation;
};

struct WaapiEventObject
{
	FString EventName;
	TArray<WaapiTargetObject> Targets;
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