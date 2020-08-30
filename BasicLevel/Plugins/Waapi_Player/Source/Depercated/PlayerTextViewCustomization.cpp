#include "PlayerTextViewCustomization.h"

#include "Widgets/Text/STextBlock.h"
#include "Widgets/SBoxPanel.h"


TSharedRef<IPropertyTypeCustomization> FPlayerTextViewCustomization::MakeInstance()
{
	return MakeShareable(new FPlayerTextViewCustomization);
}

void FPlayerTextViewCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow & HeaderRow, IPropertyTypeCustomizationUtils & CustomizationUtils)
{
}

void FPlayerTextViewCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder & ChildBuilder, IPropertyTypeCustomizationUtils & CustomizationUtils)
{
	TSharedRef<IPropertyHandle> TargetName = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWaapiTargetObject, TargetName)).ToSharedRef();
	TSharedRef<IPropertyHandle> Volume = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWaapiTargetObject, Volume)).ToSharedRef();
	TSharedRef<IPropertyHandle> Pitch = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWaapiTargetObject, Pitch)).ToSharedRef();
	TSharedRef<IPropertyHandle> LPF = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWaapiTargetObject, LPF)).ToSharedRef();
	TSharedRef<IPropertyHandle> HPF = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWaapiTargetObject, HPF)).ToSharedRef();

	TArray<TSharedRef<IPropertyHandle>> GeneralInfo = { TargetName, Volume, Pitch, LPF, HPF };

	TSharedRef<IPropertyHandle> UseListenerRelativeRoute = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWaapiTargetObject, UseListenerRelativeRoute)).ToSharedRef();
	TSharedRef<IPropertyHandle> Spatialization3D = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWaapiTargetObject, Spatialization3D)).ToSharedRef();
	TSharedRef<IPropertyHandle> MaxDistance = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWaapiTargetObject, MaxDistance)).ToSharedRef();
	TSharedRef<IPropertyHandle> UseConeAttenuation = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWaapiTargetObject, UseConeAttenuation)).ToSharedRef();

	TArray<TSharedRef<IPropertyHandle>> SpatialInfo = { UseListenerRelativeRoute, Spatialization3D, MaxDistance, UseConeAttenuation };

	TSharedRef<IPropertyHandle> UseMaxSoundInstance = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWaapiTargetObject, UseMaxSoundInstance)).ToSharedRef();
	TSharedRef<IPropertyHandle> MaxSound = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FWaapiTargetObject, MaxSound)).ToSharedRef();

	TArray<TSharedRef<IPropertyHandle>> AdvanceInfo = { UseMaxSoundInstance, MaxSound};

	TSharedPtr<SVerticalBox> GeneralWidget;
	ChildBuilder.AddCustomRow(NSLOCTEXT("DetailCustomization", "DetailCustomization", "General"))
	.NameContent()
	.ToolTipText(NSLOCTEXT("DetailCustomization", "DetailCustomization", "Ak Event General"))
	.NameContent()
	[
		SNew(STextBlock)
		.Text(NSLOCTEXT("DetailCustomization", "DetailCustomization", "General"))
		.ToolTipText(NSLOCTEXT("DetailCustomization", "DetailCustomization", "Ak Event General"))
		.Font(CustomizationUtils.GetRegularFont())
	]
	.ValueContent()
		.MaxDesiredWidget(500.f)
		[
			SAssignNew(GeneralWidget, SVerticalBox)
		];

	for (int i = 0; i < GeneralInfo.Num(); i++)
	{
		GeneralWidget->AddSlot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				GeneralInfo[i]->CreatePropertyNameWidget()
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(30.f, 0.f, 0.f, 0.f)
			[
				GeneralInfo[i]->CreatePropertyValueWidget()
			]
		];
	}
}
