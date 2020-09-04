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

TArray<UWaapiTargetObject> WaapiPlaySqlManager::QueryWaapiTargetObjects(FString EventName)
{
	if (EventName.IsEmpty() || !Conn.IsValid())
		return;

	FSQLiteResultSet* Result = nullptr;
	Conn->Execute(*(FString::Printf(TEXT("Select * from Event where Name = %s;"), *EventName)), Result);
	
	return TArray<UWaapiTargetObject>();
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
	if (Conn->IsValid())
	{
		Conn->Close();
	}
}
