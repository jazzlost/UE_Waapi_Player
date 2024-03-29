// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "WaapiPlayerSqlDatabaseConnection.h"

bool FWaapiPlayerSqlDatabaseConnection::Execute(const TCHAR* CommandString, FSQLiteResultSet*& RecordSet)
{
	RecordSet = nullptr;

	if (Database.IsValid())
	{
		// Compile the statement/query
		FSQLitePreparedStatement PreparedStatement = Database.PrepareStatement(CommandString);
		if (PreparedStatement.IsValid())
		{
			// Initialize records from compiled query
			RecordSet = new FSQLiteResultSet(MoveTemp(PreparedStatement));
			return true;
		}
	}

	return false;
}

//bool FWaapiPlayerSqlDatabaseConnection::Execute(const TCHAR* CommandString)
//{
//	return Database.IsValid() && Database.Execute(CommandString);
//}
//
//bool FWaapiPlayerSqlDatabaseConnection::Execute(const TCHAR* CommandString, FDataBaseRecordSet*& RecordSet)
//{
//	return Execute(CommandString, (FSQLiteResultSet*&)RecordSet);
//}


//bool FWaapiPlayerSqlDatabaseConnection::Open(const TCHAR* ConnectionString, const TCHAR* RemoteConnectionIP, const TCHAR* RemoteConnectionStringOverride)
//{
//	return Database.Open(ConnectionString);
//}
//
//FString FWaapiPlayerSqlDatabaseConnection::GetLastError()
//{
//	return Database.GetLastError();
//}

void FWaapiPlayerSqlDatabaseConnection::Close()
{
	 Database.Close();
}

bool FWaapiPlayerSqlDatabaseConnection::Open(const TCHAR * ConnectionString, const ESQLiteDatabaseOpenMode InOpenMode)
{
	return Database.Open(ConnectionString, InOpenMode);
}

