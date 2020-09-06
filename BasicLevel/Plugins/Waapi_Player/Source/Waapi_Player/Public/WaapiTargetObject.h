// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WaapiTargetObject.generated.h"

/**
 * 
 */

struct WaapiSwitchStateObject
{
	FString SwitchOrStateGroup;
	TArray<FString> SwitchOrState;
	bool bSwitch;
};

struct WaapiAttenObject
{
	FString AttenName;
	int MaxDistance;
	int UseConeAttenuation;
};

struct WaapiRtpcObject
{
	FString RtpcName;
	int UseBuildInParam;
	float DefaultValue;
	float MaxValue;
	float MinValue;
};

USTRUCT()
struct FWaapiTargetDataObject
{
	GENERATED_BODY()

	TSharedPtr<WaapiAttenObject> AttenObject;
	TArray<TSharedPtr<WaapiSwitchStateObject>> SwitchOrStateObjects;
	TArray<TSharedPtr<WaapiRtpcObject>> RtpcObjects;
};


UCLASS()
class WAAPIPLAYER_API UWaapiTargetObject : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	FString TargetName;

	UPROPERTY(VisibleAnywhere)
	float Volume;
	
	UPROPERTY(VisibleAnywhere)
	int Pitch;

	UPROPERTY(VisibleAnywhere)
	int LPF;

	UPROPERTY(VisibleAnywhere)
	int HPF;

	UPROPERTY(VisibleAnywhere)
	int UseMaxSoundInstance;

	UPROPERTY(VisibleAnywhere)
	int MaxSound;

	UPROPERTY(VisibleAnywhere)
	int UseListenerRelativeRoute;

	UPROPERTY(VisibleAnywhere)
	FString Spatialization3D;

	FWaapiTargetDataObject TargetExtraData;
};


struct FWaapiEventObject
{
	FString EventName;

	TArray<UWaapiTargetObject*> Targets;
};
