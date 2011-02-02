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

  Fabricio Costa > versión 2011 implementada en base a:
  http://lua-users.org/wiki/LunaFourCode
  http://lua-users.org/wiki/LunaFour

*******************************************************************************/

#ifndef __MO_LUNA_H__
#define __MO_LUNA_H__

#include "moTypes.h"

#undef check

///LunaFourCode
#include <string>
#include <vector>
#include <list>

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

  ///LunaFourCode
  enum { NUMBER, STRING };

  ///LunaFourCode
  struct PropertyType {
      const char     *name;
      int             (T::*getter) (lua_State *);
      int             (T::*setter) (lua_State *);
  };

  ///LunaFourCode
  struct FunctionType {
    const char     *name;
    int             (T::*function) (lua_State *);
  };


  ///eso no esta en LunaFourCode
	//typedef int (T::*mfp)(lua_State *L);
	//typedef struct { const char *name; mfp mfunc; } RegType;


  ///LunaFourCode
  /**
    @ check
    Arguments:
      * L - Lua State
      * narg - Position to check

    Description:
      Retrieves a wrapped class from the arguments passed to the function, specified by narg (position).
      This function will raise an exception if the argument is not of the correct type.
  */
  /*
    static T       *check(lua_State * L, int narg) {
	// Check to see whether we are a table
	    if (lua_istable(L,narg+1))
	    {
		    lua_gettablevalue(L,narg+1,0);
		    userdataType   *ud =
				    static_cast <userdataType * >(luaL_checkudata(L, -1, T::className));
		    if (!ud)
			    luaL_typerror(L, narg+1, T::className);
		    lua_pop(L,1);
		    return ud->pT;		// pointer to T object
	    }
	    else
	    {
		    luaL_typerror(L, narg+1, T::className);
	    }
    }
*/
    /** OVERRIDE OLD FUNCTION :  */
    /**
    	// get userdata from Lua stack and return pointer to T object
      static T *check(lua_State *L, int narg) {
        userdataType *ud =
          static_cast<userdataType*>(luaL_checkudata(L, narg, T::className));
        if(!ud) luaL_typerror(L, narg, T::className);
        return ud->pT;  // pointer to T object
      }
	*/


  ///LunaFourCode
  /**
    @ lightcheck
    Arguments:
      * L - Lua State
      * narg - Position to check

    Description:
      Retrieves a wrapped class from the arguments passed to the function, specified by narg (position).
      This function will return NULL if the argument is not of the correct type.  Useful for supporting
      multiple types of arguments passed to the function
  */
  /*
	static T       *lightcheck(lua_State * L, int narg) {
	// Check to see whether we are a table
	    if (lua_istable(L,narg+1))
	    {
		    lua_gettablevalue(L,narg+1,0); ///doesn't work
		    userdataType   *ud =
				    static_cast <userdataType * >(luaL_testudata(L, -1, T::className));
		    if (!ud)
			    return NULL; // lightcheck returns NULL if not found.
		    lua_pop(L,1);
		    return ud->pT;		// pointer to T object
	    }
	    else
	    {
		    return NULL;
	    }
    }
*/

	/**
     * Registra la clase T en la máquina virtual con estado L. El uso de esta función estática es:
     * moLuna<T>::Register(L);
	 * @param L puntero al estado de la máquina virtual.
	 */
	 /*
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
			// edited by Snaily: shouldn't it be const RegType *l ... ?
			lua_pushstring(L, l->name);
			lua_pushlightuserdata(L, (void*)l);
			lua_pushcclosure(L, thunk, 1);
			lua_settable(L, methods);
		}

		lua_pop(L, 2);  // drop metatable and method table
	}
*/

///LunaFourCode
/**
  @ Register
  Arguments:
    * L - Lua State
    * namespac - Namespace to load into

  Description:
    Registers your class with Lua.  Leave namespac "" if you want to load it into the global space.
*/
    // REGISTER CLASS AS A GLOBAL TABLE
    static void     Register(lua_State * L, const char *namespac = "") {

      if (strcmp(namespac, "") != 0) {
          lua_getglobal(L, namespac);
          lua_pushcfunction(L, &moLuna < T >::constructor);
          lua_setfield(L, -2, T::className);
          lua_pop(L, 1);
      } else {
          lua_pushcfunction(L, &moLuna < T >::constructor);
          lua_setglobal(L, T::className);
      }

      luaL_newmetatable(L, T::className);
      int             metatable = lua_gettop(L);

      lua_pushstring(L, "__gc");
      lua_pushcfunction(L, &moLuna < T >::gc_obj);
      lua_settable(L, metatable);

      lua_pushstring(L, "__index");
      lua_pushcfunction(L, &moLuna < T >::property_getter);
      lua_settable(L, metatable);

      lua_pushstring(L, "__setindex");
      lua_pushcfunction(L, &moLuna < T >::property_setter);
      lua_settable(L, metatable);

    }

///LunaFourCode

/**
  @ constructor (internal)
  Arguments:
    * L - Lua State
*/

    static int      constructor(lua_State * L) {

        lua_newtable(L);

        int             newtable = lua_gettop(L);

        lua_pushnumber(L, 0);

        T             **a = (T **) lua_newuserdata(L, sizeof(T *));
        //correction
        //T              *obj = new T(L, true);
        T              *obj = new T(L);
        *a = obj;

        int             userdata = lua_gettop(L);

        luaL_getmetatable(L, T::className);

        lua_setmetatable(L, userdata);

        lua_settable(L, newtable);

        luaL_getmetatable(L, T::className);
        lua_setmetatable(L, newtable);

        luaL_getmetatable(L, T::className);

        for (int i = 0; T::Properties[i].name; i++) {
            lua_pushstring(L, T::Properties[i].name);
            lua_pushnumber(L, i);
            lua_settable(L, -3);
        }

        lua_pop(L, 1);

        for (int i = 0; T::Functions[i].name; i++) {
            lua_pushstring(L, T::Functions[i].name);
            lua_pushnumber(L, i);
            lua_pushcclosure(L, &moLuna < T >::function_dispatch, 1);
            lua_settable(L, newtable);
        }

        return 1;
    }

/**
  @ createNew
  Arguments:
    * L - Lua State

  Description:
    Loads an instance of the class into the Lua stack, and provides you a pointer so you can modify it.
*/

    static T       *createNew(lua_State * L) {

      lua_newtable(L);

      int             newtable = lua_gettop(L);

      lua_pushnumber(L, 0);

      T             **a = (T **) lua_newuserdata(L, sizeof(T *));
      T              *obj = new T(L, false);
      obj->isExisting = false;
      *a = obj;

      int             userdata = lua_gettop(L);

      luaL_getmetatable(L, T::className);

      lua_setmetatable(L, userdata);

      lua_settable(L, newtable);

      luaL_getmetatable(L, T::className);
      lua_setmetatable(L, newtable);

      luaL_getmetatable(L, T::className);

      for (int i = 0; T::Properties[i].name; i++) {
          // ADD NAME KEY
          lua_pushstring(L, T::Properties[i].name);
          lua_pushnumber(L, i);
          lua_settable(L, -3);
      }

      lua_pop(L, 1);

      for (int i = 0; T::Functions[i].name; i++) {
          lua_pushstring(L, T::Functions[i].name);
          lua_pushnumber(L, i);
          lua_pushcclosure(L, &moLuna < T >::function_dispatch, 1);
          lua_settable(L, newtable);
      }

      return obj;
    }

/**
  @ createFromExisting
  Arguments:
    * L - Lua State
    * existingobj - Existing instance of object

  Description:
    Loads an instance of the class into the Lua stack, instead using an existing object rather than creating a new one.
    Returns the existing object.
*/

    static T       *createFromExisting(lua_State * L, T * existingobj) {

      lua_newtable(L);

      int newtable = lua_gettop(L);

      lua_pushnumber(L, 0);

      T **a = (T **) lua_newuserdata(L, sizeof(T *));
      T *obj = existingobj;
      obj->isExisting = true;
      *a = obj;

      int userdata = lua_gettop(L);


      luaL_getmetatable(L, T::className);

      lua_setmetatable(L, userdata);

      lua_settable(L, newtable);

      luaL_getmetatable(L, T::className);
      lua_setmetatable(L, newtable);

      luaL_getmetatable(L, T::className);

      for (int i = 0; T::Properties[i].name; i++) {
          lua_pushstring(L, T::Properties[i].name);
          lua_pushnumber(L, i);
          lua_settable(L, -3);
      }

      lua_pop(L, 1);

      for (int i = 0; T::Functions[i].name; i++) {
          lua_pushstring(L, T::Functions[i].name);
          lua_pushnumber(L, i);
          lua_pushcclosure(L, &moLuna < T >::function_dispatch, 1);
          lua_settable(L, newtable);
      }

      return obj;
    }

/**
  @ property_getter (internal)
  Arguments:
    * L - Lua State
*/

    static int      property_getter(lua_State * L) {

      lua_pushvalue(L, 2);

      lua_getmetatable(L, 1);

      lua_pushvalue(L, 2);
      lua_rawget(L, -2);

      if (lua_isnumber(L, -1)) {

          int             _index = lua_tonumber(L, -1);

          lua_pushnumber(L, 0);
          lua_rawget(L, 1);

          T             **obj =
        static_cast < T ** >(lua_touserdata(L, -1));

          lua_pushvalue(L, 3);

          const PropertyType *_properties = (*obj)->T::Properties;

        int result = ((*obj)->*(T::Properties[_index].getter)) (L);

          return result;

      }
      // PUSH NIL
      lua_pushnil(L);

      return 1;

    }

/**
  @ property_setter (internal)
  Arguments:
    * L - Lua State
*/

    static int      property_setter(lua_State * L) {

      lua_getmetatable(L, 1);

      lua_pushvalue(L, 2);
      lua_rawget(L, -2);

      if (lua_isnil(L, -1)) {

          lua_pop(L, 2);

          lua_rawset(L, 1);

          return 0;
      } else {

          int             _index = lua_tonumber(L, -1);

          lua_pushnumber(L, 0);
          lua_rawget(L, 1);

          T             **obj =
        static_cast < T ** >(lua_touserdata(L, -1));

          lua_pushvalue(L, 3);

          const PropertyType *_properties = (*obj)->T::Properties;

          return ((*obj)->*(T::Properties[_index].setter)) (L);

      }

    }

/**
  @ function_dispatch (internal)
  Arguments:
    * L - Lua State
*/

    static int  function_dispatch(lua_State * L) {

      int i = (int) lua_tonumber(L, lua_upvalueindex(1));

      lua_pushnumber(L, 0);
      lua_rawget(L, 1);

      T             **obj = static_cast < T ** >(lua_touserdata(L, -1));

      lua_pop(L, 1);

      return ((*obj)->*(T::Functions[i].function)) (L);
    }

/**
  @ gc_obj (internal)
  Arguments:
    * L - Lua State
*/

    static int      gc_obj(lua_State * L) {

      T             **obj =
          static_cast < T ** >(luaL_checkudata(L, -1, T::className));

      if (!(*obj)->isExisting && !(*obj)->isPrecious)
      {
        cout << "Cleaning up a " << T::className << "." << endl;
        delete(*obj);
      }

      return 0;
    }


private:

/*
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
	*/
};

#define DECLARE_SCRIPT_CLASS(ClassName)					\
	public:												\
	static const char className[];						\
	static moLuna<ClassName>::PropertyType Properties[]; \
	static moLuna<ClassName>::FunctionType Functions[];\
\
	bool isExisting; \
  bool isPrecious;


#define IMPLEMENT_SCRIPT_CLASS(ClassName)				\
	const char ClassName::className[] = #ClassName;

#define DEFINE_SCRIPT_CLASS_FUNCTIONS(ClassName)						\
	moLuna<ClassName>::FunctionType ClassName::Functions[] = {

#define SCRIPT_FUNCTION(ClassName, FunctionName) { #FunctionName, &ClassName::FunctionName }

#define END_SCRIPT_CLASS_FUNCTIONS };


#define DEFINE_SCRIPT_CLASS_PROPERTIES(ClassName)						\
	moLuna<ClassName>::PropertyType ClassName::Properties[] = {

#define SCRIPT_PROPERTY(ClassName, PropertyName, PropertyGetter, PropertySetter ) { #PropertyName, &ClassName::PropertyGetter, &ClassName::PropertySetter }

#define END_SCRIPT_CLASS_PROPERTIES };

#define REGISTER_CLASS(ClassName, LuaState)					\
	moLuna<ClassName>::Register(LuaState);

#define SCRIPT_CONSTRUCTOR_DECLARATION(ClassName) ClassName(lua_State* L);
#define SCRIPT_CONSTRUCTOR_IMPLEMENTATION(ClassName) ClassName::ClassName(lua_State* L)

#define SCRIPT_FUNCTION_DECLARATION(FunctionName) int FunctionName(lua_State* L);
#define SCRIPT_FUNCTION_IMPLEMENTATION(ClassName, FunctionName) int ClassName::FunctionName(lua_State* L)

#endif
