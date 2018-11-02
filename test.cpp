#include <iostream>
#include <stdio.h>
#include "ELuna.h"
#include <string>

#include "GameAI_Algorithm.h"
#include "GameAI_Test.h"
#include "GameAI_Strategy.h"
#include "GameAI_GroupType.h"
#include "GameAI_StringToClass.h"

const char* LinkTest(const char* str){
	std::string ret = "bifdfd   " + std::string(str);
	return ret.c_str();
}

//define function
const char* Interface( const char* str) {
//	const char* ret = LinkTest(str);
	const char* ret = InterfaceSo(str);
	return ret;
}

void testCPP(lua_State* L) {

	//register a function
	ELuna::registerFunction(L, "Interface", &Interface);
}


extern "C" {
int 
luaopen_libGameAI(lua_State *L){
//	lua_State *L = ELuna::openLua();

//       luaopen_base(L);
//       luaL_openlibs(L);


	testCPP(L);
//	luaL_register(L, "sample", myfuncs);	
//	ELuna::closeLua(L);

/*	luaL_checkversion(L);
	luaL_Reg l[] = {
		{ "dump", ldump },
		{ NULL, NULL },
	};

	luaL_newlib(L,l);*/
	return 1;
}
}

