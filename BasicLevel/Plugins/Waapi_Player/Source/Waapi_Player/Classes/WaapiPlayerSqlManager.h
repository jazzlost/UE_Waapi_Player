#pragma once

#include "CoreMinimal.h"
#include "../../../Plugins/Runtime/Database/SQLiteSupport/Source/SQLiteSupport/Public/SQLiteDatabaseConnection.h"

#include "WaapiTargetObject.h"

typedef FSQLiteResultSet::TIterator SqlResultIter;

enum class WaapiPlayerQueryType
{
	Event,
	Target,
	Switch,
	State,
	Atten,
	Rtpc
};

class WaapiPlaySqlManager
{
public:
	
	static WaapiPlaySqlManager& Get();
	
	WaapiPlaySqlManager();
	
	~WaapiPlaySqlManager();

	bool Init(FString DatabasePath);
	void Close();

	bool QueryWaapiTargetObjects(FString EventName, TSharedPtr<FWaapiEventObject> OutResultObject);

private:
	bool Open();

	void Query(FString ColumnName, WaapiPlayerQueryType Type, FSQLiteResultSet*& Result);

private:
	FString DatabaseFullPath;
	TSharedPtr<FSQLiteDatabaseConnection> Conn;
};

namespace TargetObjectUtil
{
	TArray<FString> SplitSqlResult(FString SqlResult);

	const TArray<FString> FillEventResult(TSharedPtr<FWaapiEventObject> OutResult, FSQLiteResultSet*& Result);
	void FillGeneralTargetResult(UWaapiTargetObject* TargetObject, SqlResultIter ResultIter);
	void FillSwitchResult(UWaapiTargetObject* TargetObject, SqlResultIter ResultIter);
	void FillStateResult(UWaapiTargetObject* TargetObject, SqlResultIter ResultIter);
	void FillAttenResult(UWaapiTargetObject* TargetObject, SqlResultIter ResultIter);
	void FillRtpcResult(UWaapiTargetObject* TargetObject, SqlResultIter ResultIter);
}
