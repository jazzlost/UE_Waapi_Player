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

	TArray<UWaapiTargetObject> QueryWaapiTargetObjects(FString EventName);

private:
	bool Open();
	void Close();
	void Query(FString EventName);
	FString DatabaseFullPath;
	TSharedPtr<FSQLiteDatabaseConnection> Conn;
};

namespace TargetObjectUtil
{

}
