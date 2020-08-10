// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "WaapiPlayerCommands.h"

#define LOCTEXT_NAMESPACE "FWaapiPlayerModule"

void FWaapiPlayerCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Waapi_Player", "Bring up Waapi_Player window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
