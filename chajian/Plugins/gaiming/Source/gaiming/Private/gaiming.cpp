// Copyright Epic Games, Inc. All Rights Reserved.

#include "gaiming.h"
#include "gaimingStyle.h"
#include "gaimingCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "Widgets/Docking/SDockTab.h"  // 可停靠面板
#include "Widgets/Layout/SBox.h"       // 布局容器
#include "Widgets/Text/STextBlock.h"   // 文本控件


// Blutility模块
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "Editor/Blutility/Public/IBlutilityModule.h"
#include "EditorUtilitySubsystem.h"
#include "Runtime/Core/Public/Modules/ModuleManager.h"
// UMG编辑器集成
#include "Editor/UMGEditor/Public/WidgetBlueprint.h"
#include "Editor/Blutility/Classes/EditorUtilityWidgetBlueprint.h"


static const FName gaimingTabName("gaiming");

#define LOCTEXT_NAMESPACE "FgaimingModule"

void FgaimingModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FgaimingStyle::Initialize();
	FgaimingStyle::ReloadTextures();

	FgaimingCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FgaimingCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FgaimingModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FgaimingModule::RegisterMenus));
}

void FgaimingModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FgaimingStyle::Shutdown();

	FgaimingCommands::Unregister();
}

void FgaimingModule::PluginButtonClicked()
{
	// 获取编辑器工具子系统
	UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();

	// 加载指定的编辑器控件蓝图
	FSoftObjectPath WidgetPath(TEXT("/Script/Blutility.EditorUtilityWidgetBlueprint'/gaiming/UI/gaiming_UI.gaiming_UI'"));
	UEditorUtilityWidgetBlueprint* WidgetBlueprint = Cast<UEditorUtilityWidgetBlueprint>(WidgetPath.TryLoad());

	if (WidgetBlueprint)
	{
		// 生成并显示编辑器控件
		EditorUtilitySubsystem->SpawnAndRegisterTab(WidgetBlueprint);
	}
	else
	{
		// 如果加载失败，显示错误信息
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to load the Editor Utility Widget Blueprint!")));
	}
}
void FgaimingModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FgaimingCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FgaimingCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FgaimingModule, gaiming)