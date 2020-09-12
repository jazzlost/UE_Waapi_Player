#include "WaapiPlayerSqlManager.h"
#include "Templates/SharedPointer.h"
#include "../../../Plugins/Runtime/Database/SQLiteCore/Source/SQLiteCore/Public/SQLiteDatabase.h"

WaapiPlaySqlManager & WaapiPlaySqlManager::Get()
{
	static WaapiPlaySqlManager Instance;
	return Instance;
}

WaapiPlaySqlManager::WaapiPlaySqlManager()
{

}

WaapiPlaySqlManager::~WaapiPlaySqlManager()
{
	Close();
}

bool WaapiPlaySqlManager::Init(FString DatabasePath)
{
	DatabaseFullPath = DatabasePath;
	return Open();
}

bool WaapiPlaySqlManager::QueryWaapiTargetObjects(FString EventName, TSharedPtr<FWaapiEventObject> OutResultObject)
{
	if (EventName.IsEmpty() || !Conn.IsValid())
		return false;

	//Query Event
	FSQLiteResultSet* Result = nullptr;
	Query(EventName, WaapiPlayerQueryType::Event, Result);

	if (Result)
	{
		const TArray<FString>& TargetsId = TargetObjectUtil::FillEventResult(OutResultObject, Result);

		//Query TargetsList
		for (auto Id : TargetsId)
		{
			//Query Target General Info
			Query(Id, WaapiPlayerQueryType::Target, Result);

			for (SqlResultIter TargetIter(Result); TargetIter; ++TargetIter)
			{
				UWaapiTargetObject* Target = NewObject<UWaapiTargetObject>();
				TargetObjectUtil::FillGeneralTargetResult(Target, TargetIter);

				//Query Switch Info
				FString SwitchGroupSql = TargetIter->GetString(TEXT("SwitchGroupId"));
				TArray<FString> SwitchGroupIds = TargetObjectUtil::SplitSqlResult(SwitchGroupSql);
				for (auto SwitchGroupId : SwitchGroupIds)
				{
					Query(SwitchGroupId, WaapiPlayerQueryType::Switch, Result);
					SqlResultIter SwitchIter(Result);
					TargetObjectUtil::FillSwitchResult(Target, SwitchIter);
				}

				//Query State Info
				FString StateGroupSql = TargetIter->GetString(TEXT("StateGroupId"));
				TArray<FString> StateGroupIds = TargetObjectUtil::SplitSqlResult(StateGroupSql);
				for (auto StateGroupId : StateGroupIds)
				{
					Query(StateGroupId, WaapiPlayerQueryType::State, Result);
					SqlResultIter StateIter(Result);
					TargetObjectUtil::FillStateResult(Target, StateIter);
				}

				//Query Atten Info
				FString AttenId = TargetIter->GetString(TEXT("AttenId"));
				Query(AttenId, WaapiPlayerQueryType::Atten, Result);
				SqlResultIter AttenIter(Result);
				TargetObjectUtil::FillAttenResult(Target, AttenIter);

				//Query RTPC Info
				FString RtpcIdSql = TargetIter->GetString(TEXT("RtpcId"));
				TArray<FString> RtpcIds = TargetObjectUtil::SplitSqlResult(RtpcIdSql);
				for (auto RtpcId : RtpcIds)
				{
					Query(RtpcId, WaapiPlayerQueryType::Rtpc, Result);
					SqlResultIter RtpcIter(Result);
					TargetObjectUtil::FillRtpcResult(Target, RtpcIter);
				}

				OutResultObject->Targets.Add(Target);
			}
		}
	}

	return true;
}


void WaapiPlaySqlManager::Query(FString ColumnName, WaapiPlayerQueryType Type, FSQLiteResultSet *& Result)
{
	if (ColumnName.IsEmpty())
		return;

	Result = nullptr;
	FString SQL;

	switch (Type)
	{
	case WaapiPlayerQueryType::Event:
	{
		SQL = FString::Printf(TEXT("SELECT * FROM Event WHERE Name='%s';"), *ColumnName);
		break;
	}
	case WaapiPlayerQueryType::Target:
	{
		SQL = FString::Printf(TEXT("SELECT * FROM Target WHERE Id='%s';"), *ColumnName);
		break;
	}
	case WaapiPlayerQueryType::Switch:
	{
		SQL = FString::Printf(TEXT("SELECT * FROM Switch WHERE Id='%s';"), *ColumnName);
		break;
	}
	case WaapiPlayerQueryType::State:
	{
		SQL = FString::Printf(TEXT("SELECT * FROM State WHERE Id='%s';"), *ColumnName);
		break;
	}
	case WaapiPlayerQueryType::Atten:
	{
		SQL = FString::Printf(TEXT("SELECT * FROM Attenuation WHERE Id='%s';"), *ColumnName);
		break;
	}
	case WaapiPlayerQueryType::Rtpc:
	{
		SQL = FString::Printf(TEXT("SELECT * FROM RTPC WHERE Id='%s';"), *ColumnName);
		break;
	}
	default:
		break;
	}

	Conn->Execute(*SQL, Result);
}

bool WaapiPlaySqlManager::Open()
{
	Conn = MakeShareable(new FSQLiteDatabaseConnection());
	return Conn->Open(*DatabaseFullPath, nullptr, nullptr);
}

void WaapiPlaySqlManager::Close()
{
	if (Conn.IsValid())
	{
		Conn->Close();
	}
}

TArray<FString> TargetObjectUtil::SplitSqlResult(FString SqlResult)
{
	TArray<FString> OutResult;
	FString Delim = TEXT(",");
	SqlResult.ParseIntoArray(OutResult, *Delim, true);

	return OutResult;
}

const TArray<FString> TargetObjectUtil::FillEventResult(TSharedPtr<FWaapiEventObject> OutResult, FSQLiteResultSet*& Result)
{
	SqlResultIter Iter(Result);
	OutResult->EventName = Iter->GetString(TEXT("Name"));

	TArray<FString> OutTargetsId;

	for (; Iter; ++Iter)
	{
		FString TargetIdString = Iter->GetString(TEXT("TargetId"));
		int TargetAction = Iter->GetInt(TEXT("ActionType"));

		TArray<FString> TargetIds = TargetObjectUtil::SplitSqlResult(TargetIdString);
		for (auto TargetId : TargetIds)
		{
			if (!TargetId.IsEmpty() && TargetAction == 1)
			{
				OutTargetsId.Add(TargetId);
				UE_LOG(LogTemp, Warning, TEXT("SQL TargetId Result: %s"), *TargetId);
			}
		}
	}
	return OutTargetsId;
}

void TargetObjectUtil::FillGeneralTargetResult(UWaapiTargetObject * TargetObject, SqlResultIter ResultIter)
{
	if (!ResultIter)
		return;

	TargetObject->TargetName = ResultIter->GetString(TEXT("Name"));
	TargetObject->Volume = ResultIter->GetFloat(TEXT("Volume"));
	TargetObject->Pitch = ResultIter->GetInt(TEXT("Pitch"));
	TargetObject->LPF = ResultIter->GetInt(TEXT("LPF"));
	TargetObject->HPF = ResultIter->GetInt(TEXT("HPF"));
	TargetObject->UseMaxSoundInstance = ResultIter->GetInt(TEXT("UseMaxSoundInstance"));
	TargetObject->MaxSound = ResultIter->GetInt(TEXT("MaxSound"));
	TargetObject->UseListenerRelativeRoute = ResultIter->GetInt(TEXT("UseListenerRelativeRoute"));
	TargetObject->Spatialization3D = ResultIter->GetString(TEXT("Spatialization3D"));
}

void TargetObjectUtil::FillSwitchResult(UWaapiTargetObject * TargetObject, SqlResultIter ResultIter)
{
	if (!ResultIter)
		return;

	for (; ResultIter; ++ResultIter)
	{
		TSharedPtr<WaapiSwitchStateObject> NewSwitchObject = MakeShareable(new WaapiSwitchStateObject());
		NewSwitchObject->SwitchOrStateGroup = ResultIter->GetString(TEXT("Name"));
		TArray<FString> Switchs = SplitSqlResult(ResultIter->GetString(TEXT("SwitchName")));
		for (auto Switch : Switchs)
		{
			TSharedPtr<FName> SwitchFName = MakeShareable(new FName(*Switch));
			NewSwitchObject->SwitchOrState.Add(SwitchFName);
		}

		NewSwitchObject->bSwitch = true;
		TargetObject->TargetExtraData.SwitchOrStateObjects.Add(NewSwitchObject);
	}
}

void TargetObjectUtil::FillStateResult(UWaapiTargetObject * TargetObject, SqlResultIter ResultIter)
{
	if (!ResultIter)
		return;

	for (; ResultIter; ++ResultIter)
	{
		TSharedPtr<WaapiSwitchStateObject> NewStateObject = MakeShareable(new WaapiSwitchStateObject());
		NewStateObject->SwitchOrStateGroup = ResultIter->GetString(TEXT("Name"));
		TArray<FString> States = SplitSqlResult(ResultIter->GetString(TEXT("StateName")));
		for (auto State : States)
		{
			TSharedPtr<FName> StateFName = MakeShareable(new FName(*State));
			NewStateObject->SwitchOrState.Add(StateFName);
		}

		NewStateObject->bSwitch = false;
		TargetObject->TargetExtraData.SwitchOrStateObjects.Add(NewStateObject);
	}
}

void TargetObjectUtil::FillAttenResult(UWaapiTargetObject * TargetObject, SqlResultIter ResultIter)
{
	if (!ResultIter)
		return;

	TargetObject->AttenName = ResultIter->GetString(TEXT("Name"));
	TargetObject->MaxDistance = ResultIter->GetInt(TEXT("MaxDistance"));
	TargetObject->UseConeAttenuation = ResultIter->GetInt(TEXT("UseConeAttenuation"));

}

void TargetObjectUtil::FillRtpcResult(UWaapiTargetObject * TargetObject, SqlResultIter ResultIter)
{
	if (!ResultIter)
		return;

	for (; ResultIter; ++ResultIter)
	{
		TSharedPtr<WaapiRtpcObject> NewRtpcObject = MakeShareable(new WaapiRtpcObject());
		NewRtpcObject->RtpcName = ResultIter->GetString(TEXT("Name"));
		NewRtpcObject->UseBuildInParam = ResultIter->GetInt(TEXT("UseBuildInParam"));
		NewRtpcObject->DefaultValue = ResultIter->GetFloat(TEXT("DefaultValue"));
		NewRtpcObject->MaxValue = ResultIter->GetFloat(TEXT("MAX"));
		NewRtpcObject->MinValue = ResultIter->GetFloat(TEXT("MIN"));

		TargetObject->TargetExtraData.RtpcObjects.Add(NewRtpcObject);
	}
}
