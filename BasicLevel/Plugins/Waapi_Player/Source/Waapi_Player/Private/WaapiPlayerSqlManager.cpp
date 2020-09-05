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
				UWaapiTargetObject* Target = NewObject<UWaapiTargetObject>();

				//Query Target General Info
				FillGeneralTargetResult(Target, Iter);

				QueryResultObjects.Add(Target);
			}
		}

		if (QueryResultObjects.Num() <= 0)
			return true;

		//Query Target General Info
		for (auto Target : QueryResultObjects)
		{
			FSQLiteResultSet* Result = nullptr;
			FString SQL = FString::Printf(TEXT("SELECT * FROM Target WHERE Name='%s';"), *TargetsId);
			Conn->Execute(*SQL, Result);
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
		int TargetAction = Iter->GetInt(TEXT("ActionType"));

		if (!TargetId.IsEmpty() && TargetAction == 1)
		{
			OutTargetsId.Add(TargetId);
			UE_LOG(LogTemp, Warning, TEXT("SQL TargetId Result: %s"), *TargetId);
		}
	}
	return OutTargetsId;
}

void TargetObjectUtil::FillGeneralTargetResult(UWaapiTargetObject * TargetObject, FSQLiteResultSet::TIterator ResultIter)
{
	TargetObject->TargetName = Iter->GetString(TEXT("Name"));
	TargetObject->Volume = Iter->GetString(TEXT("Volume"));
	TargetObject->Pitch = Iter->GetString(TEXT("Pitch"));
	TargetObject->LPF = Iter->GetString(TEXT("LPF"));
	TargetObject->HPF = Iter->GetString(TEXT("HPF"));
	TargetObject->UseMaxSoundInstance = Iter->GetString(TEXT("UseMaxSoundInstance"));
	TargetObject->MaxSound = Iter->GetString(TEXT("MaxSound"));
	TargetObject->UseListenerRelativeRoute = Iter->GetString(TEXT("UseListenerRelativeRoute"));
	TargetObject->Spatialization3D = Iter->GetString(TEXT("Spatialization3D"));
}
