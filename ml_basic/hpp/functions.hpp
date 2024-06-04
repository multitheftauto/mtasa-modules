#pragma once

#include <mta/mta_main.hpp>

class Functions {
	using FunctionType = int(*)(lua_State* luaVM);
public:
	// Your functions here in this format:
	// static int functionName(lua_State* luaVM);
};