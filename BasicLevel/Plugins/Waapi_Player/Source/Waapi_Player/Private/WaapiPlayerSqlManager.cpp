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
	Init();
}

void WaapiPlaySqlManager::Init()
{
	Open();
}

void WaapiPlaySqlManager::Open()
{
	FSQLiteDatabaseConnection Database;
	bool bOpen = Database.Open(TEXT("D:\\Project\\Code\\waapi.db"), nullptr, nullptr);
	if (!bOpen)
		return;
	FSQLitePreparedStatement* State = new FSQLitePreparedStatement();
	FSQLiteResultSet* Result = new FSQLiteResultSet(MoveTemp(*State));
	FString EventName = TEXT("Play_Ambient");
	Database.Execute(*(FString::Printf(TEXT("Select * from Event where Name = %s;"), *EventName)), Result);
	for (FSQLiteResultSet::TIterator Iter(Result); Result; ++Result)
	{
		FString ID = Iter->GetString(TEXT("Id"));
		UE_LOG(LogTemp, Warning, TEXT("SQL Result: %s"), *ID);
	}
	
	Database.Close();
}
