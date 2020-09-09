// Fill out your copyright notice in the Description page of Project Settings.


#include "WaapiPlayerPlayingObject.h"
#include "AkAudioDevice.h"
#include "AkComponent.h"

bool UWaapiPlayerPlayingObject::Playing()
{
	FAkAudioDevice* AudioDevice = FAkAudioDevice::Get();


	for (auto Pair : SwitchPair)
	{
		if (Pair.Key == "None")
			continue;

		AudioDevice->SetSwitch(*Pair.Key, *Pair.Value, nullptr);
	}

	for (auto Pair : StatePair)
	{
		if (Pair.Key == "None")
			continue;

		AudioDevice->SetState(*Pair.Key, *Pair.Value);
	}

	AudioDevice->PostEvent(EventName, nullptr, 0, NULL, NULL, false);
	//for (auto Pair : RtpcPair)
	//{
	//	AudioDevice->SetRTPCValue(*Pair.Key, Pair.Value, 0, nullptr);
	//}

	return true;
}

void UWaapiPlayerPlayingObject::SetRtpc(FString Name, float value)
{
	FAkAudioDevice* AudioDevice = FAkAudioDevice::Get();
	AudioDevice->SetRTPCValue(*Name, value, 0, nullptr);
}
