#pragma once

#include "CoreMinimal.h"
#include "../../../Plugins/Runtime/Database/SQLiteSupport/Source/SQLiteSupport/Public/SQLiteDatabaseConnection.h"

#include "WaapiTargetObject.h"

class WaapiPlaySqlManager
{
public:
	
	static WaapiPlaySqlManager& Get();
	
	WaapiPlaySqlManager();
	
	~WaapiPlaySqlManager();

	bool Init(FString DatabasePath);

	bool QueryWaapiTargetObjects(FString EventName, FWaapiEventObject& OutResultObject);

private:
	bool Open();
	void Close();

private:
	FString DatabaseFullPath;
	TSharedPtr<FSQLiteDatabaseConnection> Conn;
	TArray<UWaapiTargetObject*> QueryResultObjects;
};

namespace TargetObjectUtil
{
	const TArray<FString> FillEventResult(FWaapiEventObject& OutResult, FSQLiteResultSet* Result);
	void FillGeneralTargetResult(UWaapiTargetObject* TargetObject, FSQLiteResultSet::TIterator ResultIter);
	void FillSwitchResult(UWaapiTargetObject* TargetObject, FSQLiteResultSet* Result);
	void FillStateResult(UWaapiTargetObject* TargetObject, FSQLiteResultSet* Result);
	void FillAttenResult(UWaapiTargetObject* TargetObject, FSQLiteResultSet* Result);
}
