#pragma once

#include "CoreMinimal.h"
#include "../../../Plugins/Runtime/Database/SQLiteSupport/Source/SQLiteSupport/Public/SQLiteDatabaseConnection.h"

class WaapiPlaySqlManager
{
public:
	
	static WaapiPlaySqlManager& Get();
	
	WaapiPlaySqlManager();
	
	~WaapiPlaySqlManager(){}

	void Init(FString DatabasePath) { DatabaseFullPath = DatabasePath; }


private:
	bool Open();
	FString DatabaseFullPath;
	FSQLiteDatabaseConnection& Conn;
};
