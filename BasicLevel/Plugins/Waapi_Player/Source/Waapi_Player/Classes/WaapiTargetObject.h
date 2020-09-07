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
	FString SwitchOrStateGroup = TEXT("None");
	TArray<TSharedPtr<FName>> SwitchOrState;
	bool bSwitch = true;
};

struct WaapiAttenObject
{
	FString AttenName = TEXT("None");
	int MaxDistance = 0;
	int UseConeAttenuation = 0;
};

struct WaapiRtpcObject
{
	FString RtpcName = TEXT("None");
	int UseBuildInParam = 0;
	float DefaultValue = 0.f;
	float MaxValue = 1.f;
	float MinValue = 0.f;
};

USTRUCT()
struct FWaapiTargetDataObject
{
	GENERATED_BODY()

	//TSharedPtr<WaapiAttenObject> AttenObject;
	TArray<TSharedPtr<WaapiSwitchStateObject>> SwitchOrStateObjects;
	TArray<TSharedPtr<WaapiRtpcObject>> RtpcObjects;
};


UCLASS()
class WAAPIPLAYER_API UWaapiTargetObject : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	FString TargetName = TEXT("None");

	UPROPERTY(VisibleAnywhere)
	float Volume = 0.f;
	
	UPROPERTY(VisibleAnywhere)
	int Pitch = 0;

	UPROPERTY(VisibleAnywhere)
	int LPF = 0;

	UPROPERTY(VisibleAnywhere)
	int HPF = 0;

	UPROPERTY(VisibleAnywhere)
	int UseMaxSoundInstance = 0;

	UPROPERTY(VisibleAnywhere)
	int MaxSound = 0;

	UPROPERTY(VisibleAnywhere)
	int UseListenerRelativeRoute = 0;

	UPROPERTY(VisibleAnywhere)
	FString Spatialization3D = TEXT("None");

	UPROPERTY(VisibleAnywhere)
	FString AttenName = TEXT("None");

	UPROPERTY(VisibleAnywhere)
	int MaxDistance = 0;

	UPROPERTY(VisibleAnywhere)
	int UseConeAttenuation = 0;

	FWaapiTargetDataObject TargetExtraData;
};

USTRUCT()
struct FWaapiEventObject
{
	GENERATED_BODY()

	FWaapiEventObject();

	FString EventName = TEXT("None");
	TArray<UWaapiTargetObject*> Targets;
};
