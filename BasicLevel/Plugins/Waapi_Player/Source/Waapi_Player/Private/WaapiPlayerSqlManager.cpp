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

bool WaapiPlaySqlManager::QueryWaapiTargetObjects(FString EventName, FWaapiEventObject& OutResultObject)
{
	if (EventName.IsEmpty() || !Conn.IsValid())
		return false;

	//Query Event
	FSQLiteResultSet* Result = nullptr;
	QueryEvent(EventName, Result);

	if (Result)
	{
		const TArray<FString>& TargetsId = TargetObjectUtil::FillEventResult(OutResultObject, Result);

		//Query TargetsList
		QueryResultObjects.Empty();
		for (auto Id : TargetsId)
		{
			//Query Target General Info
			QueryTarget(Id, Result);

			for (SqlResultIter TargetIter(Result); TargetIter; ++TargetIter)
			{
				UWaapiTargetObject* Target = NewObject<UWaapiTargetObject>();
				TargetObjectUtil::FillGeneralTargetResult(Target, TargetIter);

				//Query Switch Info
				FString SwitchGroupSql = TargetIter->GetString(TEXT("SwitchGroupId"));
				TArray<FString> SwitchGroupIds = TargetObjectUtil::SplitSqlResult(SwitchGroupSql);
				for (auto SwitchGroupId : SwitchGroupIds)
				{
					QuerySwitch(SwitchGroupId, Result);
					SqlResultIter SwitchIter(Result);
					TargetObjectUtil::FillSwitchResult(Target, SwitchIter);
				}

				//Query State Info
				FString StateGroupSql = TargetIter->GetString(TEXT("StateGroupId"));
				TArray<FString> StateGroupIds = TargetObjectUtil::SplitSqlResult(StateGroupSql);
				for (auto StateGroupId : StateGroupIds)
				{
					QueryState(StateGroupId, Result);
					SqlResultIter StateIter(Result);
					TargetObjectUtil::FillStateResult(Target, StateIter);
				}

				//Query Atten Info
				FString AttenId = TargetIter->GetString(TEXT("AttenId"));
				QueryAtten(AttenId, Result);
				SqlResultIter AttenIter(Result);
				TargetObjectUtil::FillAttenResult(Target, AttenIter);

				//Query RTPC Info
				FString RtpcIdSql = TargetIter->GetString(TEXT("RtpcId"));
				TArray<FString> RtpcIds = TargetObjectUtil::SplitSqlResult(RtpcIdSql);
				for (auto RtpcId : RtpcIds)
				{
					QueryRtpc(RtpcId, Result);
					SqlResultIter RtpcIter(Result);
					TargetObjectUtil::FillRtpcResult(Target, RtpcIter);
				}

				QueryResultObjects.Add(Target);
			}
		}

		if (QueryResultObjects.Num() <= 0)
			return true;
	}

	return true;
}


bool WaapiPlaySqlManager::Open()
{
	Conn = MakeShareable(new FSQLiteDatabaseConnection());
	return Conn->Open(*DatabaseFullPath, nullptr, nullptr);

	//FSQLiteResultSet* Result = nullptr;
	//FString EventName = TEXT("Play_Ambient");
	//Database.Execute(*(FString::Printf(TEXT("Select * from Event where Name = %s;"), *EventName)), Result);

	//Conn->Execute(*(FString::Printf(TEXT("SELECT * FROM waapiTable WHERE value=6.0"))), Result);
	//for (FSQLiteResultSet::TIterator Iter(Result); Iter; ++Iter)
	//{
	//	FString ID = Iter->GetString(TEXT("keyword"));
	//	UE_LOG(LogTemp, Warning, TEXT("SQL Result: %s"), *ID);
	//}
	//Conn->Close();
}

void WaapiPlaySqlManager::Close()
{
	if (Conn.IsValid())
	{
		Conn->Close();
	}
}

void WaapiPlaySqlManager::QueryEvent(FString EventName, FSQLiteResultSet *& Result)
{
	Result = nullptr;
	FString SQL = FString::Printf(TEXT("SELECT * FROM Event WHERE Name='%s';"), *EventName);
	Conn->Execute(*SQL, Result);
}

void WaapiPlaySqlManager::QueryTarget(FString TargetId, FSQLiteResultSet *& Result)
{
	Result = nullptr;
	FString SQL = FString::Printf(TEXT("SELECT * FROM Target WHERE Id='%s';"), *TargetId);
	Conn->Execute(*SQL, Result);
}

void WaapiPlaySqlManager::QuerySwitch(FString SwitchId, FSQLiteResultSet *& Result)
{
	Result = nullptr;
	FString SQL = FString::Printf(TEXT("SELECT * FROM Switch WHERE Id='%s';"), *SwitchId);
	Conn->Execute(*SQL, Result);
}

void WaapiPlaySqlManager::QueryState(FString StateGroupId, FSQLiteResultSet *& Result)
{
}

void WaapiPlaySqlManager::QueryAtten(FString StateGroupId, FSQLiteResultSet *& Result)
{
}

void WaapiPlaySqlManager::QueryRtpc(FString RtpcId, FSQLiteResultSet *& Result)
{
}


TArray<FString> TargetObjectUtil::SplitSqlResult(FString SqlResult)
{
	return TArray<FString>();
}

const TArray<FString> TargetObjectUtil::FillEventResult(FWaapiEventObject& OutResult, FSQLiteResultSet*& Result)
{
	SqlResultIter Iter(Result);
	OutResult.EventName = Iter->GetString(TEXT("Name"));

	TArray<FString> OutTargetsId;

	for (; Iter; ++Iter)
	{
		FString TargetId = Iter->GetString(TEXT("TargetId"));
		int TargetAction = Iter->GetInt(TEXT("ActionType"));

		if (!TargetId.IsEmpty() && TargetAction == 1)
		{
			OutTargetsId.Add(TargetId);
			UE_LOG(LogTemp, Warning, TEXT("SQL TargetId Result: %s"), *TargetId);
		}
	}
	return OutTargetsId;
}

void TargetObjectUtil::FillGeneralTargetResult(UWaapiTargetObject * TargetObject, SqlResultIter ResultIter)
{
	TargetObject->TargetName = ResultIter->GetString(TEXT("Name"));
	TargetObject->Volume = ResultIter->GetString(TEXT("Volume"));
	TargetObject->Pitch = ResultIter->GetString(TEXT("Pitch"));
	TargetObject->LPF = ResultIter->GetString(TEXT("LPF"));
	TargetObject->HPF = ResultIter->GetString(TEXT("HPF"));
	TargetObject->UseMaxSoundInstance = ResultIter->GetString(TEXT("UseMaxSoundInstance"));
	TargetObject->MaxSound = ResultIter->GetString(TEXT("MaxSound"));
	TargetObject->UseListenerRelativeRoute = ResultIter->GetString(TEXT("UseListenerRelativeRoute"));
	TargetObject->Spatialization3D = ResultIter->GetString(TEXT("Spatialization3D"));
}

void TargetObjectUtil::FillSwitchResult(UWaapiTargetObject * TargetObject, SqlResultIter ResultIter)
{
}

void TargetObjectUtil::FillStateResult(UWaapiTargetObject * TargetObject, SqlResultIter ResultIter)
{
}

void TargetObjectUtil::FillAttenResult(UWaapiTargetObject * TargetObject, SqlResultIter ResultIter)
{
}

void TargetObjectUtil::FillRtpcResult(UWaapiTargetObject * TargetObject, SqlResultIter ResultIter)
{
}
