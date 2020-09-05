// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WaapiTargetObject.generated.h"

/**
 * 
 */
UCLASS()
class WAAPIPLAYER_API UWaapiTargetObject : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	FString TargetName;

	UPROPERTY(VisibleAnywhere)
	FString Volume;
	
	UPROPERTY(VisibleAnywhere)
	FString Pitch;

	UPROPERTY(VisibleAnywhere)
	FString LPF;

	UPROPERTY(VisibleAnywhere)
	FString HPF;

	UPROPERTY(VisibleAnywhere)
	FString UseMaxSoundInstance;

	UPROPERTY(VisibleAnywhere)
	FString MaxSound;

	UPROPERTY(VisibleAnywhere)
	FString UseListenerRelativeRoute;

	UPROPERTY(VisibleAnywhere)
	FString Spatialization3D;

	UPROPERTY(VisibleAnywhere)
	FString MaxDistance;

	UPROPERTY(VisibleAnywhere)
	FString UseConeAttenuation;

	WaapiTargetDataObject TargetExtraData;
};

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


struct FWaapiEventObject
{
	FString EventName;

	TArray<UWaapiTargetObject*> Targets;
};


struct WaapiTargetDataObject
{
	TArray<WaapiSwitchStateObject> SwitchOrStateObjects;
	TArray<WaapiRtpcObject> RtpcObjects;
};
