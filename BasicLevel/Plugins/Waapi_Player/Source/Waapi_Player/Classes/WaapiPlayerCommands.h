// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "WaapiPlayerStyle.h"

class FWaapiPlayerCommands : public TCommands<FWaapiPlayerCommands>
{
public:

	FWaapiPlayerCommands()
		: TCommands<FWaapiPlayerCommands>(TEXT("Waapi_Player"), NSLOCTEXT("Contexts", "Waapi_Player", "Waapi_Player Plugin"), NAME_None, FWaapiPlayerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};