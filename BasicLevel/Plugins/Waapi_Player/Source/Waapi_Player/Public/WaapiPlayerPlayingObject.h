// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WaapiPlayerPlayingObject.generated.h"

/**
 * 
 */
class UAkComponent;

UCLASS()
class WAAPIPLAYER_API UWaapiPlayerPlayingObject : public UObject
{
	GENERATED_BODY()

public:
	
	FString EventName;
	
	TMap<FString, FString> SwitchPair;
	
	TMap<FString, FString> StatePair;
	
	TMap<FString, float> RtpcPair;
	
public:

	bool Playing();
	
	void SetRtpc(FString Name, float value);
};
