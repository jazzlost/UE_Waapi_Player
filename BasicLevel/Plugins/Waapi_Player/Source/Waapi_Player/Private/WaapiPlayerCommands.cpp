// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "WaapiPlayerCommands.h"

#define LOCTEXT_NAMESPACE "FWaapiPlayerModule"

void FWaapiPlayerCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "WaapiPlayer", "Bring up WaapiPlayer window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
