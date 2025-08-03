// Copyright Epic Games, Inc. All Rights Reserved.

#include "gaimingCommands.h"

#define LOCTEXT_NAMESPACE "FgaimingModule"

void FgaimingCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "gaiming", "Execute gaiming action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
