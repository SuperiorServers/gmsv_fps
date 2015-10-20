// gmsv_fps
// made by thelastpenguin

#include "GarrysMod/Lua/Interface.h"

#include "engine/iserverplugin.h"
#include "game/server/iplayerinfo.h"

#ifdef _LINUX
	#define SERVER_LIB "./garrysmod/bin/server_srv.so"
#else
	#define SERVER_LIB "server.dll"
#endif


CGlobalVars *g_GlobalVars = NULL;
IPlayerInfoManager *g_PlayerInfoManager = NULL;

using namespace GarrysMod::Lua;

int FrameCount( lua_State* state )
{
	LUA->PushNumber((float)g_GlobalVars->framecount);
	
	return 1;
}

int RealFrameTime( lua_State* state )
{
	LUA->PushNumber((float)g_GlobalVars->absoluteframetime);
	
	return 1;
}

GMOD_MODULE_OPEN()
{
	CreateInterfaceFn serverFactory = Sys_GetFactory(SERVER_LIB);

	if (!serverFactory) {
		LUA->ThrowError("error getting serverFactory!! (basically fuck you from Garry)");
		return 0;
	}

	g_PlayerInfoManager = (IPlayerInfoManager *)serverFactory(INTERFACEVERSION_PLAYERINFOMANAGER, NULL);
	if (!g_PlayerInfoManager) {
		LUA->ThrowError("error getting g_PlayerInfoManager!! (still basically fuck you from Garry)");
		return 0;
	}

	g_GlobalVars = g_PlayerInfoManager->GetGlobalVars();

	LUA->PushSpecial( GarrysMod::Lua::SPECIAL_GLOB );

	LUA->GetField(-1, "engine");
	
	LUA->PushCFunction( FrameCount );
	LUA->SetField( -2, "FrameCount" );
	
	LUA->PushCFunction( RealFrameTime );
	LUA->SetField( -2, "RealFrameTime" );

	LUA->Pop(2);

	return 0;
}

GMOD_MODULE_CLOSE()
{
	return 0;
}
