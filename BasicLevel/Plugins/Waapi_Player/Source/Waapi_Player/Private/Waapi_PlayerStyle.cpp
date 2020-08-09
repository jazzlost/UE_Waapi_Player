// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Waapi_PlayerStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Engine/Texture2D.h"


TSharedPtr< FSlateStyleSet > FWaapi_PlayerStyle::StyleInstance = NULL;

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )

#define WAAPI_TEXTURE( RelativePath, TextureName ) LoadObject<UTexture2D>(nullptr, *(FString("/Waapi_Player/") / FString(RelativePath) / FString(TextureName) + FString(".") + FString(TextureName)))
#define WAAPI_IMAGE_BRUSH( Texture, ... ) FSlateImageBrush( Texture, __VA_ARGS__ )


void SetWaapiBrush(FSlateStyleSet& Style, const char* BrushName, const char* TextureName)
{
	const FVector2D Icon15x15(15.0f, 15.0f);
	const char* AkIconsPath = "WaapiTree/Icons";

	auto Texture = WAAPI_TEXTURE(AkIconsPath, TextureName);
	if (Texture != nullptr)
	{
		Texture->AddToRoot();
		Style.Set(BrushName, new WAAPI_IMAGE_BRUSH(Texture, Icon15x15));
	}
}

void SetResourceBrushes(FSlateStyleSet& Style)
{
	SetWaapiBrush(Style, "WaapiPlayer.ActorMixerIcon", "actor_mixer_nor");
	SetWaapiBrush(Style, "WaapiPlayer.SoundIcon", "sound_fx_nor");
	SetWaapiBrush(Style, "WaapiPlayer.SwitchContainerIcon", "container_switch_nor");
	SetWaapiBrush(Style, "WaapiPlayer.RandomSequenceContainerIcon", "container_random_sequence_nor");
	SetWaapiBrush(Style, "WaapiPlayer.BlendContainerIcon", "layer_container_nor");
	SetWaapiBrush(Style, "WaapiPlayer.MotionBusIcon", "motion_bus_nor");
	SetWaapiBrush(Style, "WaapiPlayer.AkPickerTabIcon", "wwise_logo_32");
	SetWaapiBrush(Style, "WaapiPlayer.EventIcon", "event_nor");
	SetWaapiBrush(Style, "WaapiPlayer.AcousticTextureIcon", "acoutex_nor");
	SetWaapiBrush(Style, "WaapiPlayer.AuxBusIcon", "auxbus_nor");
	SetWaapiBrush(Style, "WaapiPlayer.BusIcon", "bus_nor");
	SetWaapiBrush(Style, "WaapiPlayer.FolderIcon", "folder_nor");
	SetWaapiBrush(Style, "WaapiPlayer.PhysicalFolderIcon", "physical_folder_nor");
	SetWaapiBrush(Style, "WaapiPlayer.WorkUnitIcon", "workunit_nor");
	SetWaapiBrush(Style, "WaapiPlayer.ProjectIcon", "wproj");
	SetWaapiBrush(Style, "WaapiPlayer.RTPCIcon", "gameparameter_nor");
}

void FWaapi_PlayerStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();

		//auto ContentRoot = FPaths::EngineContentDir() / TEXT("Slate");
		//StyleInstance->SetContentRoot(ContentRoot);
		//StyleInstance->SetCoreContentRoot(ContentRoot);

		FSlateStyleSet& Style = *StyleInstance.Get();
		{
			SetResourceBrushes(Style);

			//Style.Set("AudiokineticTools.GroupBorder", new ENGINE_BOX_BRUSH("Common/GroupBorder", FMargin(4.0f / 16.0f)));
			//Style.Set("AudiokineticTools.AssetDragDropTooltipBackground", new ENGINE_BOX_BRUSH("Old/Menu_Background", FMargin(8.0f / 64.0f)));

		//	FButtonStyle HoverHintOnly = FButtonStyle()
		//		.SetNormal(FSlateNoResource())
		//		.SetHovered(ENGINE_BOX_BRUSH("Common/Button_Hovered", FMargin(4 / 16.0f), FLinearColor(1, 1, 1, 0.15f)))
		//		.SetPressed(ENGINE_BOX_BRUSH("Common/Button_Pressed", FMargin(4 / 16.0f), FLinearColor(1, 1, 1, 0.25f)))
		//		.SetNormalPadding(FMargin(0, 0, 0, 1))
		//		.SetPressedPadding(FMargin(0, 1, 0, 0));
		//	Style.Set("AudiokineticTools.HoverHintOnly", HoverHintOnly);

		//	Style.Set("AudiokineticTools.SourceTitleFont", ENGINE_TTF_CORE_FONT("Fonts/Roboto-Regular", 12));
		//	Style.Set("AudiokineticTools.SourceTreeItemFont", ENGINE_TTF_CORE_FONT("Fonts/Roboto-Regular", 10));
		//	Style.Set("AudiokineticTools.SourceTreeRootItemFont", ENGINE_TTF_CORE_FONT("Fonts/Roboto-Regular", 12));

			//const FVector2D Icon12x12(12.0f, 12.0f);
			//Style.Set("WaapiPlayer.Button_EllipsisIcon", new ENGINE_IMAGE_BRUSH("Icons/ellipsis_12x", Icon12x12));
		}

		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FWaapi_PlayerStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FWaapi_PlayerStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("Waapi_PlayerStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);

TSharedRef< FSlateStyleSet > FWaapi_PlayerStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("Waapi_PlayerStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("Waapi_Player")->GetBaseDir() / TEXT("Resources"));

	Style->Set("Waapi_Player.OpenPluginWindow", new IMAGE_BRUSH(TEXT("ButtonIcon_40x"), Icon40x40));

	return Style;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT

void FWaapi_PlayerStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FWaapi_PlayerStyle::Get()
{
	return *StyleInstance;
}




const FSlateBrush* FWaapi_PlayerStyle::GetBrush(EWaapiPlayerTreeItemType::Type ItemType)
{
	auto& Style = Get();
	switch (ItemType)
	{
	case EWaapiPlayerTreeItemType::Type::Event: return Style.GetBrush("WaapiPlayer.EventIcon");
	//case EWwiseTreeItemType::AcousticTexture: return Style.GetBrush("AudiokineticTools.AcousticTextureIcon");
	//case EWwiseTreeItemType::AuxBus: return Style.GetBrush("AudiokineticTools.AuxBusIcon");
	//case EWwiseTreeItemType::Bus: return Style.GetBrush("AudiokineticTools.BusIcon");
	case EWaapiPlayerTreeItemType::Type::Folder: return Style.GetBrush("WaapiPlayer.FolderIcon");
	case EWaapiPlayerTreeItemType::Type::Project: return Style.GetBrush("WaapiPlayer.ProjectIcon");
	//case EWwiseTreeItemType::PhysicalFolder: return Style.GetBrush("AudiokineticTools.PhysicalFolderIcon");
	//case EWwiseTreeItemType::StandaloneWorkUnit:
	//case EWwiseTreeItemType::NestedWorkUnit: return Style.GetBrush("AudiokineticTools.WorkUnitIcon");
	//case EWwiseTreeItemType::ActorMixer: return Style.GetBrush("AudiokineticTools.ActorMixerIcon");
	//case EWwiseTreeItemType::Sound: return Style.GetBrush("AudiokineticTools.SoundIcon");
	//case EWwiseTreeItemType::SwitchContainer: return Style.GetBrush("AudiokineticTools.SwitchContainerIcon");
	//case EWwiseTreeItemType::RandomSequenceContainer: return Style.GetBrush("AudiokineticTools.RandomSequenceContainerIcon");
	//case EWwiseTreeItemType::BlendContainer: return Style.GetBrush("AudiokineticTools.BlendContainerIcon");
	//case EWwiseTreeItemType::MotionBus: return Style.GetBrush("AudiokineticTools.MotionBusIcon");
	default:
		return nullptr;
	}
}
