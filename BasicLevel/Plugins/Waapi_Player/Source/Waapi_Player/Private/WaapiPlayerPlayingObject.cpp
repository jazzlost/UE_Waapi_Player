// Fill out your copyright notice in the Description page of Project Settings.


#include "WaapiPlayerPlayingObject.h"
#include "AkAudioDevice.h"
#include "AkComponent.h"

bool UWaapiPlayerPlayingObject::Playing()
{
	if (Component == nullptr)
	{
		Component = FAkAudioDevice::Get()->GetAkComponent();
	}

	AudioDevice->PostEvent(EventName, nullptr);

	for (auto Pair : SwitchPair)
	{
		AudioDevice->SetSwitch(*Pair.Key, *Pair.Value, nullptr);
	}

	for (auto Pair : StatePair)
	{
		AudioDevice->SetState(*Pair.Key, *Pair.Value);
	}

	for (auto Pair : RtpcPair)
	{
		AudioDevice->SetRTPCValue(*Pair.Key, Pair.Value, 0, nullptr);
	}

	return true;
}
