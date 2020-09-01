#include "WaapiPlayerSqlManager.h"
#include "Templates/SharedPointer.h"

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
}

void WaapiPlaySqlManager::Open()
{
	FSQLiteDatabase Database;
	Database.Open()
}
