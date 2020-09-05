#pragma once

#include "CoreMinimal.h"
#include "../../../Plugins/Runtime/Database/SQLiteSupport/Source/SQLiteSupport/Public/SQLiteDatabaseConnection.h"

#include "WaapiTargetObject.h"

typedef FSQLiteResultSet::TIterator SqlResultIter;

class WaapiPlaySqlManager
{
public:
	
	static WaapiPlaySqlManager& Get();
	
	WaapiPlaySqlManager();
	
	~WaapiPlaySqlManager();

	bool Init(FString DatabasePath);
	void Close();

	bool QueryWaapiTargetObjects(FString EventName, FWaapiEventObject& OutResultObject);

private:
	bool Open();

	void QueryEvent(FString EventName, FSQLiteResultSet*& Result);
	void QueryTarget(FString TargetId, FSQLiteResultSet*& Result);
	void QuerySwitch(FString SwitchGroupId, FSQLiteResultSet*& Result);
	void QueryState(FString StateGroupId, FSQLiteResultSet*& Result);
	void QueryAtten(FString StateGroupId, FSQLiteResultSet*& Result);
	void QueryRtpc(FString RtpcId, FSQLiteResultSet*& Result);

private:
	FString DatabaseFullPath;
	TSharedPtr<FSQLiteDatabaseConnection> Conn;
	TArray<UWaapiTargetObject*> QueryResultObjects;

};

namespace TargetObjectUtil
{
	TArray<FString> SplitSqlResult(FString SqlResult);

	const TArray<FString> FillEventResult(FWaapiEventObject& OutResult, FSQLiteResultSet*& Result);
	void FillGeneralTargetResult(UWaapiTargetObject* TargetObject, SqlResultIter ResultIter);
	void FillSwitchResult(UWaapiTargetObject* TargetObject, SqlResultIter ResultIter);
	void FillStateResult(UWaapiTargetObject* TargetObject, SqlResultIter ResultIter);
	void FillAttenResult(UWaapiTargetObject* TargetObject, SqlResultIter ResultIter);
	void FillRtpcResult(UWaapiTargetObject* TargetObject, SqlResultIter ResultIter);
}
