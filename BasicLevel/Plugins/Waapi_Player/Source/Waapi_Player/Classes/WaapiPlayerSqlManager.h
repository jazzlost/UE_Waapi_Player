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

struct WaapiQuaryObject
{
	FString EventName;
};

struct WaapiDataObject
{
	TArray<WaapiSwitchStateObject> SwitchOrStateObjects;
	TArray<WaapiRtpcObject> RtpcObjects;
};