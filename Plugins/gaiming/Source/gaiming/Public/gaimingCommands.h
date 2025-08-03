// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "gaimingStyle.h"

class FgaimingCommands : public TCommands<FgaimingCommands>
{
public:

	FgaimingCommands()
		: TCommands<FgaimingCommands>(TEXT("gaiming"), NSLOCTEXT("Contexts", "gaiming", "gaiming Plugin"), NAME_None, FgaimingStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
