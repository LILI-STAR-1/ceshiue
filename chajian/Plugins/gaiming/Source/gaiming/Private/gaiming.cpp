// Copyright Epic Games, Inc. All Rights Reserved.

#include "gaiming.h"
#include "gaimingStyle.h"
#include "gaimingCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "Widgets/Docking/SDockTab.h"  // ��ͣ�����
#include "Widgets/Layout/SBox.h"       // ��������
#include "Widgets/Text/STextBlock.h"   // �ı��ؼ�


// Blutilityģ��
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "Editor/Blutility/Public/IBlutilityModule.h"
#include "EditorUtilitySubsystem.h"
#include "Runtime/Core/Public/Modules/ModuleManager.h"
// UMG�༭������
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
	// ��ȡ�༭��������ϵͳ
	UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();

	// ����ָ���ı༭���ؼ���ͼ
	FSoftObjectPath WidgetPath(TEXT("/Script/Blutility.EditorUtilityWidgetBlueprint'/gaiming/UI/gaiming_UI.gaiming_UI'"));
	UEditorUtilityWidgetBlueprint* WidgetBlueprint = Cast<UEditorUtilityWidgetBlueprint>(WidgetPath.TryLoad());

	if (WidgetBlueprint)
	{
		// ���ɲ���ʾ�༭���ؼ�
		EditorUtilitySubsystem->SpawnAndRegisterTab(WidgetBlueprint);
	}
	else
	{
		// �������ʧ�ܣ���ʾ������Ϣ
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