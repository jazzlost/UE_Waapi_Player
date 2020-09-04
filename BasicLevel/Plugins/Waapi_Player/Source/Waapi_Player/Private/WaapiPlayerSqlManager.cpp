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


bool WaapiPlaySqlManager::Open()
{
	Conn = FSQLiteDatabaseConnection New_Conn;
	Conn.Open(*DatabaseFullPath, nullptr, nullptr);

	//FSQLiteResultSet* Result = nullptr;
	//FString EventName = TEXT("Play_Ambient");
	//Database.Execute(*(FString::Printf(TEXT("Select * from Event where Name = %s;"), *EventName)), Result);

	//Database.Execute(*(FString::Printf(TEXT("SELECT * FROM Event WHERE Name='Play_Ambient'"))), Result);
	//for (FSQLiteResultSet::TIterator Iter(Result); Iter; ++Iter)
	//{
	//	FString ID = Iter->GetString(TEXT("Id"));
	//	UE_LOG(LogTemp, Warning, TEXT("SQL Result: %s"), *ID);
	//}
	
	//Database.Close();
}
