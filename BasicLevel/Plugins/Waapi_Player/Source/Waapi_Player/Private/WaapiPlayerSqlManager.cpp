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
	FString SQL = FString::Printf(TEXT("SELECT * FROM Event WHERE Name='%s';"), *EventName);
	Conn->Execute(*SQL, Result);

	if (Result)
	{
		const TArray<FString>& TargetsId = TargetObjectUtil::FillEventResult(OutResultObject, Result);

		//Query TargetsList
		QueryResultObjects.Empty();
		for (auto Id : TargetsId)
		{
			FSQLiteResultSet* Result = nullptr;
			FString SQL = FString::Printf(TEXT("SELECT * FROM Target WHERE Id='%s';"), *Id);
			Conn->Execute(*SQL, Result);

			for (FSQLiteResultSet::TIterator Iter(Result); Iter; ++Iter)
			{
				FString TargetName = Iter->GetString(TEXT("Name"));
				UWaapiTargetObject* Target = NewObject<UWaapiTargetObject>();
				QueryResultObjects.Add(Target);
			}
		}
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

const TArray<FString> TargetObjectUtil::FillEventResult(FWaapiEventObject& OutResult, FSQLiteResultSet* Result)
{
	FSQLiteResultSet::TIterator Iter(Result);
	OutResult.EventName = Iter->GetString(TEXT("Name"));

	TArray<FString> OutTargetsId;

	for (FSQLiteResultSet::TIterator Iter(Result); Iter; ++Iter)
	{
		FString TargetId = Iter->GetString(TEXT("TargetId"));
		if (!TargetId.IsEmpty())
		{
			OutTargetsId.Add(TargetId);
			UE_LOG(LogTemp, Warning, TEXT("SQL TargetId Result: %s"), *TargetId);
		}
	}
	return OutTargetsId;
}
