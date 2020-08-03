// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Waapi_PlayerCommands.h"

#define LOCTEXT_NAMESPACE "FWaapi_PlayerModule"

void FWaapi_PlayerCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Waapi_Player", "Bring up Waapi_Player window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
