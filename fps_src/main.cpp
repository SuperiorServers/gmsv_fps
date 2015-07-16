#include "GarrysMod/Lua/Interface.h"
#include <stdio.h>

#include "edict.h"
#include "eiface.h" // engine interface.h IVEngineServer needs this
#include "engine/iserverplugin.h"
#include "game/server/iplayerinfo.h"

#include "Color.h"
#include "tier0/dbg.h"

#ifdef _LINUX
	#define SERVER_LIB "server.so"
	#define ENGINE_LIB "engine.so"
#else
	#define SERVER_LIB "server.dll"
	#define ENGINE_LIB "engine.dll"
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
		LUA->ThrowError("error getting serverFactory!!");
		return 0;
	}

	g_PlayerInfoManager = (IPlayerInfoManager *)serverFactory(INTERFACEVERSION_PLAYERINFOMANAGER, NULL);

	if (!g_PlayerInfoManager) {
		LUA->ThrowError("error getting g_PlayerInfoManager!!");
		return 0;
	}
	
	ConColorMsg(Color(255, 255, 255, 255), "gmsv_fps by ");
	ConColorMsg(Color(0, 50, 255, 255), "thelastpenguin\n");
	ConColorMsg(Color(100, 100, 100, 255), "\tBuilt on " __DATE__ " at " __TIME__ "\n");

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
