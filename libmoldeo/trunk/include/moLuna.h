/*******************************************************************************

                                  moLuna.h

  ****************************************************************************
  *                                                                          *
  *   This source is free software; you can redistribute it and/or modify    *
  *   it under the terms of the GNU General Public License as published by   *
  *   the Free Software Foundation; either version 2 of the License, or      *
  *  (at your option) any later version.                                     *
  *                                                                          *
  *   This code is distributed in the hope that it will be useful, but       *
  *   WITHOUT ANY WARRANTY; without even the implied warranty of             *
  *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      *
  *   General Public License for more details.                               *
  *                                                                          *
  *   A copy of the GNU General Public License is available on the World     *
  *   Wide Web at <http://www.gnu.org/copyleft/gpl.html>. You can also       *
  *   obtain it by writing to the Free Software Foundation,                  *
  *   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.         *
  *                                                                          *
  ****************************************************************************

  Copyright(C) 2006 Fabricio Costa

  Authors:
  Fabricio Costa
  Andrés Colubri

  Description:
  Implementa un método para agregar clases de moldeo a la máquina virtual de LUA,
  de manera tal que puedan ser creadas y accedidas desde los scripts. Para mas
  información al respecto:
  http://lua-users.org/wiki/SimpleCppBinding
  http://lua-users.org/wiki/SimplerCppBinding
  http://lua-users.org/wiki/CppBindingWithLunar
  Cada link describe un método diferente, el que se utiliza aquí es el segundo.

*******************************************************************************/

#ifndef __MO_LUNA_H__
#define __MO_LUNA_H__

extern "C"
{
   #include "lua.h"
   #include "lauxlib.h"
   #include "lualib.h"
}

/**
 * Esta clase template estática es utilizada para registrar clases de Moldeo dentro de la
 * máquina virtual de LUA.
 */
template <typename T> class moLuna {
	typedef struct { T *pT; } userdataType;
public:
	typedef int (T::*mfp)(lua_State *L);
	typedef struct { const char *name; mfp mfunc; } RegType;

	/**
     * Registra la clase T en la máquina virtual con estado L. El uso de esta función estática es:
     * moLuna<T>::Register(L);
	 * @param L puntero al estado de la máquina virtual.
	 */
	static void Register(lua_State *L) {
		lua_newtable(L);
		int methods = lua_gettop(L);

		luaL_newmetatable(L, T::className);
		int metatable = lua_gettop(L);

		// store method table in globals so that
		// scripts can add functions written in Lua.
		lua_pushstring(L, T::className);
		lua_pushvalue(L, methods);
		lua_settable(L, LUA_GLOBALSINDEX);

		lua_pushliteral(L, "__metatable");
		lua_pushvalue(L, methods);
		lua_settable(L, metatable);  // hide metatable from Lua getmetatable()

		lua_pushliteral(L, "__index");
		lua_pushvalue(L, methods);
		lua_settable(L, metatable);

		lua_pushliteral(L, "__tostring");
		lua_pushcfunction(L, tostring_T);
		lua_settable(L, metatable);

		lua_pushliteral(L, "__gc");
		lua_pushcfunction(L, gc_T);
		lua_settable(L, metatable);

		lua_newtable(L);                // mt for method table
		int mt = lua_gettop(L);
		lua_pushliteral(L, "__call");
		lua_pushcfunction(L, new_T);
		lua_pushliteral(L, "new");
		lua_pushvalue(L, -2);           // dup new_T function
		lua_settable(L, methods);       // add new_T to method table
		lua_settable(L, mt);            // mt.__call = new_T
		lua_setmetatable(L, methods);

		// fill method table with methods from class T
		for (RegType *l = T::methods; l->name; l++) {
			/* edited by Snaily: shouldn't it be const RegType *l ... ? */
			lua_pushstring(L, l->name);
			lua_pushlightuserdata(L, (void*)l);
			lua_pushcclosure(L, thunk, 1);
			lua_settable(L, methods);
		}

		lua_pop(L, 2);  // drop metatable and method table
	}

	// get userdata from Lua stack and return pointer to T object
	static T *check(lua_State *L, int narg) {
		userdataType *ud =
			static_cast<userdataType*>(luaL_checkudata(L, narg, T::className));
		if(!ud) luaL_typerror(L, narg, T::className);
		return ud->pT;  // pointer to T object
	}

private:
	moLuna();  // hide default constructor

	// member function dispatcher
	static int thunk(lua_State *L) {
		// stack has userdata, followed by method args
		T *obj = check(L, 1);  // get 'self', or if you prefer, 'this'
		lua_remove(L, 1);  // remove self so member function args start at index 1
		// get member function from upvalue
		RegType *l = static_cast<RegType*>(lua_touserdata(L, lua_upvalueindex(1)));
		return (obj->*(l->mfunc))(L);  // call member function
	}

	// create a new T object and
	// push onto the Lua stack a userdata containing a pointer to T object
	static int new_T(lua_State *L) {
		lua_remove(L, 1);   // use classname:new(), instead of classname.new()
		T *obj = new T(L);  // call constructor for T objects
		userdataType *ud =
			static_cast<userdataType*>(lua_newuserdata(L, sizeof(userdataType)));
		ud->pT = obj;  // store pointer to object in userdata
		luaL_getmetatable(L, T::className);  // lookup metatable in Lua registry
		lua_setmetatable(L, -2);
		return 1;  // userdata containing pointer to T object
	}

	// garbage collection metamethod
	static int gc_T(lua_State *L) {
		userdataType *ud = static_cast<userdataType*>(lua_touserdata(L, 1));
		T *obj = ud->pT;
		if ( obj )
		{
			delete obj;  // call destructor for T objects
			obj = 0;
		}
		return 0;
	}

	static int tostring_T (lua_State *L) {
		char buff[32];
		userdataType *ud = static_cast<userdataType*>(lua_touserdata(L, 1));
		T *obj = ud->pT;
		sprintf(buff, "%p", obj);
		lua_pushfstring(L, "%s (%s)", T::className, buff);
		return 1;
	}
};

#define DECLARE_SCRIPT_CLASS(ClassName)					\
	public:												\
	static const char className[];						\
	static moLuna<ClassName>::RegType methods[];

#define IMPLEMENT_SCRIPT_CLASS(ClassName)				\
	const char ClassName::className[] = #ClassName;

#define DEFINE_SCRIPT_CLASS(ClassName)						\
	moLuna<ClassName>::RegType ClassName::methods[] = {

#define SCRIPT_METHOD(ClassName, MethodName) { #MethodName, &ClassName::MethodName }

#define END_SCRIPT_CLASS };

#define REGISTER_CLASS(ClassName, LuaState)					\
	moLuna<ClassName>::Register(LuaState);

#define SCRIPT_CONSTRUCTOR_DECLARATION(ClassName) ClassName(lua_State* L);
#define SCRIPT_CONSTRUCTOR_IMPLEMENTATION(ClassName) ClassName::ClassName(lua_State* L)

#define SCRIPT_FUNCTION_DECLARATION(FunctionName) int FunctionName(lua_State* L);
#define SCRIPT_FUNCTION_IMPLEMENTATION(ClassName, FunctionName) int ClassName::FunctionName(lua_State* L)

#endif
