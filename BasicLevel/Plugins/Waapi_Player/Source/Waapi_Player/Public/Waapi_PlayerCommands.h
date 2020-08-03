// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "Waapi_PlayerStyle.h"

class FWaapi_PlayerCommands : public TCommands<FWaapi_PlayerCommands>
{
public:

	FWaapi_PlayerCommands()
		: TCommands<FWaapi_PlayerCommands>(TEXT("Waapi_Player"), NSLOCTEXT("Contexts", "Waapi_Player", "Waapi_Player Plugin"), NAME_None, FWaapi_PlayerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};