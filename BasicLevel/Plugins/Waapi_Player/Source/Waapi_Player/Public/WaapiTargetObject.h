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

	UPROPERTY(EditAnywhere)
	FString TargetName;

	UPROPERTY(EditAnywhere)
	FString Volume;
	
	UPROPERTY(EditAnywhere)
	FString Pitch;

	UPROPERTY(EditAnywhere)
	FString LPF;

	UPROPERTY(BlueprintReadOnly)
	FString HPF;

	UPROPERTY(BlueprintReadOnly)
	FString UseMaxSoundInstance;

	UPROPERTY(BlueprintReadOnly)
	FString MaxSound;

	UPROPERTY(BlueprintReadOnly)
	FString UseListenerRelativeRoute;

	UPROPERTY(BlueprintReadOnly)
	FString Spatialization3D;

	UPROPERTY(BlueprintReadOnly)
	FString MaxDistance;

	UPROPERTY(BlueprintReadOnly)
	FString UseConeAttenuation;
	
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

struct WaapiQuaryObject
{
	FString EventName;
};

struct WaapiDataObject
{
	TArray<WaapiSwitchStateObject> SwitchOrStateObjects;
	TArray<WaapiRtpcObject> RtpcObjects;
};
